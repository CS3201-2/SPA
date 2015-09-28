#include "Checker.h"

const regex assignmentRegex("(([[:alpha:]])([[:alnum:]]+)*)=(.*);\\}*");
const regex procDeclarationRegex("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{");
const regex procCallRegex("call(([[:alpha:]])([[:alnum:]]+)*);\\}*");
const regex whileRegex("while(([[:alpha:]])([[:alnum:]]+)*)\\{");
const regex ifRegex("if(([[:alpha:]])([[:alnum:]]+)*)then\\{");
const regex elseRegex("else\\{");
const regex variableRegex("(^[[:alpha:]])([[:alnum:]]+)*$");
const int assignmentStmt = 0;
const int procDeclarationStmt = 1;
const int procCallStmt = 2;
const int whileStmt = 3;
const int ifStmt = 4;
const int elseStmt = 5;
const int invalidStmt = 6;

Checker::Checker() {

}

bool Checker::isSyntaxCorrect(list<pair<int, string>>& sourceList) {
	stack<int> brackets; // must be empty after this for loop
	list<string> calledProcList;
	list<string> procedureList;
	stack<pair<int, string>> ifStack; //must be empty after the for loop
	string currentProc;
	list<stack<int>> bracketList;

	for (list<pair<int, string>>::iterator it = sourceList.begin(); it != sourceList.end(); ++it) {
		string stmt = (*it).second;
		int stmtType = getTypeOfStmt(stmt);
		
		switch (stmtType) {
		case assignmentStmt: 
			if (!popBrackets(brackets, stmt)) { return false; }
			if (!isAssignmentValid(stmt)) { return false; } // add variable to VarTable
			break;

		case procDeclarationStmt:
			// update current procedure
			if (brackets.empty()) {
				currentProc = getProcName(stmtType, stmt);
			}
			else {
				return false;
			}
			//update brackets;
			brackets.push(1);

			//update procedure list
			if (find(procedureList.begin(), procedureList.end(), getProcName(stmtType, stmt)) == procedureList.end()) {
				procedureList.push_back(getProcName(stmtType, stmt));
			}
			else {
				return false;
			}
			break;

		case procCallStmt:
			if (!popBrackets(brackets, stmt)) { return false; }

			//update called procedure list, if it is not current procedure
			if (currentProc != getProcName(stmtType, stmt)) {
				calledProcList.push_back(getProcName(stmtType, stmt));
			}
			else { 
				return false; 
			}
			//add procedure into ProcTable
			break;

		case whileStmt: 
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currentProc, bracketList)) {
				return false;
			}

			break;

		case ifStmt: 
			// update if stack if it is empty, or it means if without else
			if (ifStack.empty()) {
				ifStack.push(*it);
			}
			else {
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currentProc, bracketList)) {
				return false;
			}
			break;

		case elseStmt: 
			//pop only when there is one in the if stack, or it might means else without if
			if (ifStack.size() == 1) {
				(*it).first = ifStack.top().first;
				(*it).second = ifStack.top().second;
				ifStack.pop();
			}
			else {
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currentProc, bracketList)) {
				return false;
			}

			break;

		case invalidStmt: 
			return false; 
			break;
		}
	}

	// check matching brackets
	if (!brackets.empty()) {
		//cout << "bracket" << endl;
		return false;
	}

	// check matching if else statement
	if (!ifStack.empty()) {
		//cout << "if stack" << endl;
		return false;
	}

	for (list<string>::iterator it = calledProcList.begin(); it != calledProcList.end(); ++it) {
		if (find(procedureList.begin(), procedureList.end(), *it) == procedureList.end()) {
			return false;
		}
	}

	return true;
}

string Checker::getProcName(int stmtType, string str) {
	smatch m;
	if (stmtType == procDeclarationStmt) {
		regex_search(str, m, procDeclarationRegex);
	}
	else {
		regex_search(str, m, procCallRegex);
	}
	return m[1];
}

bool Checker::processNestedStmt(list<pair<int, string>>::iterator& it, list<pair<int, string>>& sourceList,
	stack<int>& brackets, list<string>& calledProcList, string currentProc, list<stack<int>>& bracketList) {
	
	stack<int> forBracketList;
	forBracketList.push(1);
	bracketList.push_back(forBracketList);

	stack<pair<int, string>> ifStack; //must be empty after the for loop
	brackets.push(1);

	++it; //to skip the starting of this nested statement
	while (!bracketList.back().empty()) {

		for (int i = 0; i < countNumOfLeftBraces((*it).second); ++i) {
			for (list<stack<int>>::iterator iter = bracketList.begin(); iter != bracketList.end(); ++iter) {
				if (!(*iter).empty()) {
					(*iter).push(1);
				}
			}
		}
		for (int i = 0; i < countNumOfRightBraces((*it).second); ++i) {
			for (list<stack<int>>::iterator iter1 = bracketList.begin(); iter1 != bracketList.end(); ++iter1) {
				if (!(*iter1).empty()) {
					(*iter1).pop();
				}
			}
		}

		string stmt = (*it).second;
		int stmtType = getTypeOfStmt(stmt);

		switch (stmtType) {
		case assignmentStmt:
			if (!popBrackets(brackets, stmt)) { return false; }
			if (!isAssignmentValid(stmt)) { return false; } // add variable to VarTable
			break;

		case procCallStmt:
			if (!popBrackets(brackets, stmt)) { return false; }

			//update called procedure list, if it is not current procedure
			if (currentProc != getProcName(stmtType, stmt)) {
				calledProcList.push_back(getProcName(stmtType, stmt));
			}
			else {
				return false;
			}
			//add procedure into ProcTable
			break;

		case whileStmt:
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currentProc, bracketList)) {
				return false;
			}
			break;

		case ifStmt:
			// update if stack if it is empty, or it means if without else
			if (ifStack.empty()) {
				ifStack.push(*it);
			}
			else {
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currentProc, bracketList)) {
				return false;
			}
			break;

		case elseStmt:
			//pop only when there is one in the if stack, or it means else without if
			if (ifStack.size() == 1) {
				(*it).first = ifStack.top().first;
				(*it).second = ifStack.top().second;
				ifStack.pop();
			}
			else {
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currentProc, bracketList)) {
				return false;
			}
			break;

		case invalidStmt:
			return false;
			break;
		}

		if (!bracketList.back().empty()) {
			++it;
		}
	}
	bracketList.pop_back();

	// check matching if else statement
	if (!ifStack.empty()) {
		return false;
	}

	return true;
}

int Checker::getTypeOfStmt(string str) {
	if (regex_match(str, assignmentRegex)) {
		return assignmentStmt;
	}
	else if (regex_match(str, procDeclarationRegex)) {
		return procDeclarationStmt;
	}
	else if (regex_match(str, procCallRegex)) {
		return procCallStmt;
	}
	else if (regex_match(str, whileRegex)) {
		return whileStmt;
	}
	else if (regex_match(str, ifRegex)) {
		return ifStmt;
	}
	else if (regex_match(str, elseRegex)) {
		return elseStmt;
	}
	else {
		return invalidStmt;
	}
}

bool Checker::isAssignmentValid(string str) {
	smatch m;
	regex_search(str, m, assignmentRegex);
	string LHS = m[1];
	string RHS = m[4];

	if (!regex_match(LHS, variableRegex)) {
		return false;
	}

	string variable;

	for (string::iterator it = RHS.begin(); it != RHS.end(); ++it) {
		if (isParenthesis(*it) || isOperator(*it)) {
			if (variable != "") {
				if (!isConstant(variable) && !isVariable(variable)) {
					return false;
				}
			}
			else {
				return false;
			}
			variable = "";
		}
		else {
			variable += *it;
		}
	}

	if (variable != "") {
		if (!isConstant(variable) && !isVariable(variable)) {
			return false;
		}
	}
	else {
		return false;
	}

	return true;
}

bool Checker::isConstant(string str) {
	size_t found = str.find_first_not_of("1234567890");
	return (found == string::npos);
}

bool Checker::isSemicolon(char ch) {
	return ch == ';';
}

bool Checker::isParenthesis(char ch) {
	return ch == '(' || ch == ')';
}

//test whether a char is + - * /
bool Checker::isOperator(char ch) {
	switch (ch) {
	case '+': case'-': case'*':
		return true;
	default:
		return false;
	}
}

bool Checker::isVariable(string str) {
	return regex_match(str, variableRegex);
}

int Checker::countNumOfLeftBraces(string str) {
	return count(str.begin(), str.end(), '{');
}

int Checker::countNumOfRightBraces(string str) {
	return count(str.begin(), str.end(), '}');
}

bool Checker::popBrackets(stack<int>& brackets, string stmt) {
	for (int i = 0; i < countNumOfRightBraces(stmt); ++i) {
		if (!brackets.empty()) {
			brackets.pop();
		}
		else {
			return false;
		}
	}
	return true;
}