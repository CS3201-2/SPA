#include "SPALog.h"
#include "Checker.h"

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
		int stmtNo = (*it).first;

		switch (stmtType) {
		case assignmentStmt: 
			if (!popBrackets(brackets, stmt)) {
				SPALog::log("miss match of {}");
				return false;
			}
			if (!isAssignmentValid(stmt)) {
				SPALog::log("invalid assign: " + stmt);
				return false;
			}
			break;


		case procDeclarationStmt:
			// update current procedure
			if (brackets.empty()) {
				procName = getProcName(stmtType, stmt);
				if (PKB::getPKBInstance()->getProcID(procName) == 0) {
					currProcID = PKB::getPKBInstance()->insertProc(procName);
				}
				else {
					SPALog::log("there is already a procedure with this name" + procName); 
					return false;
				}

				if (!isValidName(procName)) {
					SPALog::log("invalid procName: " + stmt);
					return false;
				}
			}
			else {
				SPALog::log("extra {" + stmt);
				return false;
			}

			brackets.push(1);
			break;


		case procCallStmt:
			if (!popBrackets(brackets, stmt)) {
				SPALog::log("missing { before" + stmt); 
				return false;
			}
			procName = getProcName(stmtType, stmt);
			if (!isValidName(procName)) {
				SPALog::log("invalid procName: " + stmt);
				return false;
			}
			calledPair = make_pair(currProcID, procName);
			calledProcList.push_back(calledPair);
			break;


		case whileStmt: 
			if (!isValidName(getControlVarName(stmtType, stmt))) {
				SPALog::log("invalid control variable: "+ stmt);
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;


		case ifStmt:
			if (!isValidName(getControlVarName(stmtType, stmt))) {
				SPALog::log("invalid control variable: " + stmt);
				return false;
			}
			// update if stack if it is empty, or it means if without else
			if (ifStack.empty()) {
				ifStack.push(*it);
			}
			else {
				SPALog::log("an if without else before: " + stmt); 
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
				SPALog::log("no if before this else"); 
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				SPALog::log(to_string(stmtNo)); 
				return false;
			}
			break;


		default:
			SPALog::log("invalid stmt: " + stmt);
			return false; 
			break;
		}
	}

	// check matching brackets
	if (!brackets.empty()) {
		SPALog::log("additional {");
		return false;
	}

	// check matching if else statement
	if (!ifStack.empty()) {
		SPALog::log("extrax if stmt");
		return false;
	}

	//checker gets the callsMap and callsStarMap and validate them
	DesignExtractor de = DesignExtractor();
	de.setCalls(calledProcList);
	if (!isCallValid()) {
		SPALog::log("calls itself directly or call non-exist proc"); 
		return false;
	}
	
	de.setCallsStar();
	if (!isCallsStarValid()) { 
		SPALog::log("calls itself indirectly"); 
		return false; 
	}
	return true;
}

bool Checker::isCallValid() {
	map<int, list<int>> callsMap = PKB::getPKBInstance()->getCallsMap();
	for (map<int, list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		int first = (*it).first;
		list<int> secondList = (*it).second;
		for (list<int>::iterator it2 = secondList.begin(); it2 != secondList.end(); ++it2) {
			if (*it2 == first || *it2 == 0) {
				return false;
			}
		}
	}

	return true;
}

bool Checker::isCallsStarValid() {
	map<int, list<int>> callsStarMap = PKB::getPKBInstance()->getCallsStarMap();
	for (map<int, list<int>>::iterator it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
		int first = (*it).first;
		list<int> secondList = (*it).second;
		if (find(secondList.begin(), secondList.end(), first) != secondList.end()) {
			return false;
		}
	}

	return true;
}

string Checker::getProcName(int stmtType, string stmt) {
	size_t i;
	if (stmtType == procCallStmt) {
		i = stmt.find(";");
		return stmt.substr(4, i - 4);
	}
	else {
		i = stmt.find("{");
		return stmt.substr(9, i - 9);
	}
}

string Checker::getControlVarName(int stmtType, string stmt) {
	size_t i;
	if (stmtType == whileStmt) {
		i = stmt.find("{");
		return stmt.substr(5, i - 5);
	}
	else {
		i = stmt.find("{");
		return stmt.substr(2, i - 6);
	}
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

		int stmtNo = (*it).first;
		string stmt = (*it).second;
		int stmtType = getTypeOfStmt(stmt);

		switch (stmtType) {
		case assignmentStmt:
			if (!popBrackets(brackets, stmt)) { 
				SPALog::log("miss match of {}"); 
				return false; 
			}
			if (!isAssignmentValid(stmt)) { 
				SPALog::log("invalid assign at" + stmtNo); 
				return false;
			}
			break;


		case procCallStmt:
			if (!popBrackets(brackets, stmt)) { 
				SPALog::log("missing { before" + stmt);
				return false;
			}
			if (!isValidName(procName)) {
				SPALog::log("invalid procName: " + stmt);
				return false;
			}
			procName = getProcName(stmtType, stmt);
			calledPair = make_pair(currProcID, procName);
			calledProcList.push_back(calledPair);
			break;


		case whileStmt:
			if (!isValidName(getControlVarName(stmtType, stmt))) {
				SPALog::log("invalid control variable: " + stmt);
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;


		case ifStmt:
			if (!isValidName(getControlVarName(stmtType, stmt))) {
				SPALog::log("invalid control variable: " + stmt);
				return false;
			}
			// update if stack if it is empty, or it means if without else
			if (ifStack.empty()) {
				ifStack.push(*it);
			}
			else {
				SPALog::log("an if without else before " + stmt);
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
				SPALog::log("no if before this else");
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;

		default:
			SPALog::log("invalid stmt: " + stmt);
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
		SPALog::log("extra if");
		return false;
	}

	return true;
}


bool Checker::isAssignmentValid(string assignStmt) {
	vector<string> tokens = parseExpression(assignStmt);
	string LHS = tokens.at(0);
	
	if (!isValidName(LHS)) {
		SPALog::log("LHS of expression is not a variable: "+ assignmentStmt);
		return false;
	}

	if (isOperator(tokens.at(2))) {
		SPALog::log("expression starts with an operator: " + assignmentStmt);
		return false;
	}

	string prevToken = "dummy";
	for (int i = 2; i < tokens.size(); ++i) {
		if (!isOperator(tokens.at(i)) && !isValidName(tokens.at(i)) && 
			!isParenthesis(tokens.at(i)) && !isConstant(tokens.at(i))) {
			SPALog::log("most probably, this case is invalid variable name at RHS of expression: "
				+ assignmentStmt);
			return false;
		}

		if (isOperator(prevToken) && isOperator(tokens.at(i))) {
			SPALog::log("two consecutive operators in an assignment: " + assignmentStmt);
			return false;
		}
		prevToken = tokens.at(i);
	}

	if (countNumOfLeftParenthesis(assignStmt) != countNumOfRightParenthesis(assignStmt)) {
		SPALog::log("unmatch of ()" + assignmentStmt);
		return false;
	}

	return true;
}

int Checker::countNumOfLeftBraces(string stmt) {
	return count(stmt.begin(), stmt.end(), '{');
}

int Checker::countNumOfRightBraces(string stmt) {
	return count(stmt.begin(), stmt.end(), '}');
}

int Checker::countNumOfLeftParenthesis(string stmt) {
	return count(stmt.begin(), stmt.end(), '(');
}
int Checker::countNumOfRightParenthesis(string stmt) {
	return count(stmt.begin(), stmt.end(), ')');
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

int Checker::getTypeOfStmt(string str) {
	size_t i, j;

	if (count(str.begin(), str.end(), '=') == 1) {
		i = str.find(";");
		j = str.find("=");
		if (isValidName(str.substr(0, j))
			&& isAllClosingBraces(str.substr(i + 1))) {
			return assignmentStmt;
		}
		else {
			return invalidStmt;
		}
	}

	if (str.substr(0, 9) == "procedure") {
		i = str.find("{");
		if (isValidName(str.substr(9, i - 9))) {
			return procDeclarationStmt;
		}
		else {
			return invalidStmt;
		}
	}

	if (str.substr(0, 4) == "call") {
		i = str.find(";");
		if (isValidName(str.substr(4, i - 4)) &&
			isAllClosingBraces(str.substr(i + 1))) {
			return procCallStmt;
		}
		else {
			return invalidStmt;
		}
	}

	if (str.substr(0, 5) == "while") {
		i = str.find("{");
		if (isValidName(str.substr(5, i - 5))) {
			return whileStmt;
		}
		else {
			return invalidStmt;
		}
	}

	if (str.substr(0, 2) == "if") {
		i = str.find("{");
		if (str.substr(i - 4, 4) == "then" && isValidName(str.substr(2, i - 6))) {
			return ifStmt;
		}
		else {
			return invalidStmt;
		}
	}

	if (str == "else{") {
		return elseStmt;
	}

	return invalidStmt;
}

bool Checker::isConstant(string str) {
	for (char& c : str) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

bool Checker::isValidName(string str) {
	if (!isalpha(str.at(0))) {
		return false;
	}

	for (char& c : str) {
		if (!isalnum(c)) {
			return false;
		}
	}

	if (str == "if" || str == "else" || str == "while" || 
		str == "procedure" || str == "then" || str == "call") {
		return false;
	}

	return true;
}

bool Checker::isAllClosingBraces(string str) {
	if (!str.empty()) {
		return str.find_first_not_of("}") == string::npos;
	}

	return true;
}

vector<string> Checker::parseExpression(string expression) {
	size_t found = expression.find_first_of("(=+-*);");
	vector<string> result;
	string temp;

	while (found != string::npos) {
		temp = expression.substr(0, found);
		if (temp != "") {
			result.push_back(temp);
		}

		temp = expression.at(found);
		if (temp != ";") {
			result.push_back(temp);
		}

		expression = expression.substr(found + 1);
		found = expression.find_first_of("=+-*();");
	}

	return result;
}

bool Checker::isParenthesis(string str) {
	return str == "(" || str == ")";
}

bool Checker::isOperator(string str) {
	return str == "+" || str == "-" || str == "*";
}