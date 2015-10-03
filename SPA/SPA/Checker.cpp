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
	pair<int, string> calledPair;
	list<pair<int, string>> calledProcList; //int is the currProcID, string is the calledProcName
	stack<pair<int, string>> ifStack; //must be empty after the for loop
	int currProcID;
	string procName;
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
				procName = getProcName(stmtType, stmt);
				if (PKB::getPKBInstance()->getProcID(procName) == 0) {
					currProcID = PKB::getPKBInstance()->insertProc(procName);
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
			//update brackets;
			brackets.push(1);
			break;

		case procCallStmt:
			if (!popBrackets(brackets, stmt)) { return false; }
			procName = getProcName(stmtType, stmt);
			//update called procedure list, if it is not current procedure
			if (currProcID != PKB::getPKBInstance()->getProcID(procName)) {
				calledPair = make_pair(currProcID, procName);
				calledProcList.push_back(calledPair);
			}
			else { 
				return false; 
			}
			//add procedure into ProcTable
			break;

		case whileStmt: 
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
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
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
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
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
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

	//construction of calls and callsStar should be moved under design extractor (should also be a singleton pattern)
	//checker gets the callsMap and callsStarMap and validate them
	if (!isCallValid(calledProcList)) { return false; }
	PKB::getPKBInstance()->sortAndUnifyCallsMap();
	if (!isCallsStarValid()) { return false; }
	PKB::getPKBInstance()->sortAndUnifyCallsStarMap();

	return true;
}

bool Checker::isCallsStarValid() {
	map<int, list<int>> callsMap = PKB::getPKBInstance()->getCallsMap();
	bool isValid = true;
	
	//loop through all the first in callsMap
	for (map<int, list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		int first = (*it).first;
		if (PKB::getPKBInstance()->getCallsStarSecond(first).size() == 0) {
			list<int> firstList;
			firstList.push_back(first);
			processCallsStar(isValid, firstList);
		}
	}

	return isValid;
}

void Checker::processCallsStar(bool& isValid, list<int> firstList) {
	int first = firstList.back();
	list<int> secondList = PKB::getPKBInstance()->getCallsSecond(first);
	
	for (list<int>::iterator it = secondList.begin(); it != secondList.end(); ++it) {
		list<int> tempFirstList = firstList;
		if (find(tempFirstList.begin(), tempFirstList.end(), *it) == tempFirstList.end()) {
			for (list<int>::iterator tfit = tempFirstList.begin(); tfit != tempFirstList.end(); ++tfit) {
				PKB::getPKBInstance()->setCallsStar(*tfit, *it);
			}
		}
		else {
			isValid = false;
			return;
		}
		tempFirstList.push_back(*it);
		processCallsStar(isValid, tempFirstList);
	}
}

bool Checker::isCallValid(list<pair<int, string>> calledProcList) {
	for (list<pair<int, string>>::iterator it = calledProcList.begin(); it != calledProcList.end(); ++it) {
		int callFirstID = (*it).first;
		int callSecondID = PKB::getPKBInstance()->getProcID((*it).second);

		if (callSecondID == 0) {
			return false;
		}
		else {
			PKB::getPKBInstance()->setCalls(callFirstID, callSecondID);
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
	stack<int>& brackets, list<pair<int, string>>& calledProcList, int currProcID, list<stack<int>>& bracketList) {
	
	stack<int> forBracketList;
	forBracketList.push(1);
	bracketList.push_back(forBracketList);
	pair<int, string> calledPair;
	string procName;

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

			if (!popBrackets(brackets, stmt)) { return false; }
			procName = getProcName(stmtType, stmt);
			//update called procedure list, if it is not current procedure
			if (currProcID != PKB::getPKBInstance()->getProcID(procName)) {
				calledPair = make_pair(currProcID, procName);
				calledProcList.push_back(calledPair);
			}
			else {
				return false;
			}
			break;

		case whileStmt:
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
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
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
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
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
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