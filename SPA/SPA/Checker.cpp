#include "SPALog.h"
#include "Checker.h"

Checker::Checker() {
}

bool Checker::isSyntaxCorrect(list<Statement>& sourceList) {
	stack<int> brackets; // must be empty after this for loop
	pair<int, string> calledPair;
	list<pair<int, string>> calledProcList; //int is the currProcID, string is the calledProcName
	stack<Statement> ifStack; //must be empty after the for loop
	int currProcID;
	string procName;
	list<stack<int>> bracketList;

	for (list<Statement>::iterator it = sourceList.begin(); it != sourceList.end(); ++it) {
		string stmtContent = (*it).getContent();
		StatementType stmtType = (*it).getType();
		int stmtNo = (*it).getNumber();

		switch (stmtType) {
		case assignmentStmt: 
			if (!popBrackets(brackets, stmtContent)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "miss match of {}");
				return false;
			}
			if (!isAssignmentValid(stmtContent)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid assign: " + stmtContent);
				return false;
			}
			break;


		case procDeclarationStmt:
			// update current procedure
			if (brackets.empty()) {
				procName = getProcName(stmtType, stmtContent);
				if (PKB::getPKBInstance()->getProcID(procName) == 0) {
					currProcID = PKB::getPKBInstance()->insertProc(procName);
				}
				else {
					SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "there is already a procedure with this name" + procName);
					return false;
				}

				if (!isValidName(procName)) {
					SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid procName: " + stmtContent);
					return false;
				}
			}
			else {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "extra {" + stmtContent);
				return false;
			}

			brackets.push(1);
			break;


		case procCallStmt:
			if (!popBrackets(brackets, stmtContent)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "missing { before" + stmtContent);
				return false;
			}
			procName = getProcName(stmtType, stmtContent);
			if (!isValidName(procName)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid procName: " + stmtContent);
				return false;
			}
			calledPair = make_pair(currProcID, procName);
			calledProcList.push_back(calledPair);
			break;


		case whileStmt: 
			if (!isValidName(getControlVarName(stmtType, stmtContent))) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid control variable: "+ stmtContent);
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;


		case ifStmt:
			if (!isValidName(getControlVarName(stmtType, stmtContent))) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid control variable: " + stmtContent);
				return false;
			}
			// update if stack if it is empty, or it means if without else
			if (ifStack.empty()) {
				ifStack.push(*it);
			}
			else {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "an if without else before: " + stmtContent);
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;


		case elseStmt: 
			//pop only when there is one in the if stack, or it might means else without if
			if (ifStack.size() == 1) {
				(*it).setNumber(ifStack.top().getNumber());
				(*it).setContent(ifStack.top().getContent());
				(*it).setType(ifStack.top().getType());
				ifStack.pop();
			}
			else {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "no if before this else");
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, to_string(stmtNo));
				return false;
			}
			break;


		default:
			SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid stmt: " + stmtContent);
			return false; 
			break;
		}
	}

	// check matching brackets
	if (!brackets.empty()) {
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "additional {");
		return false;
	}

	// check matching if else statement
	if (!ifStack.empty()) {
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "extrax if stmt");
		return false;
	}

	//checker gets the callsMap and callsStarMap and validate them
	DesignExtractor de = DesignExtractor();
	de.setCalls(calledProcList);
	if (!isCallValid()) {
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "calls itself directly or call non-exist proc");
		return false;
	}
	
	de.setCallsStar();
	if (!isCallsStarValid()) { 
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "calls itself indirectly");
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

string Checker::getProcName(StatementType stmtType, string stmt) {
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

string Checker::getControlVarName(StatementType stmtType, string stmt) {
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


bool Checker::processNestedStmt(list<Statement>::iterator& it, list<Statement>& sourceList,
	stack<int>& brackets, list<pair<int, string>>& calledProcList, int currProcID, list<stack<int>>& bracketList) {
	
	stack<int> forBracketList;
	forBracketList.push(1);
	bracketList.push_back(forBracketList);
	pair<int, string> calledPair;
	string procName;

	stack<Statement> ifStack; //must be empty after the for loop
	brackets.push(1);

	++it; //to skip the starting of this nested statement
	while (!bracketList.back().empty()) {
		for (int i = 0; i < countNumOfLeftBraces((*it).getContent()); ++i) {
			for (list<stack<int>>::iterator iter = bracketList.begin(); iter != bracketList.end(); ++iter) {
				if (!(*iter).empty()) {
					(*iter).push(1);
				}
			}
		}
		for (int i = 0; i < countNumOfRightBraces((*it).getContent()); ++i) {
			for (list<stack<int>>::iterator iter1 = bracketList.begin(); iter1 != bracketList.end(); ++iter1) {
				if (!(*iter1).empty()) {
					(*iter1).pop();
				}
			}
		}

		int stmtNo = (*it).getNumber();
		string stmtContent = (*it).getContent();
		StatementType stmtType = (*it).getType();

		switch (stmtType) {
		case assignmentStmt:
			if (!popBrackets(brackets, stmtContent)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "miss match of {}");
				return false; 
			}
			if (!isAssignmentValid(stmtContent)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid assign at" + stmtNo);
				return false;
			}
			break;


		case procCallStmt:
			if (!popBrackets(brackets, stmtContent)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "missing { before" + stmtContent);
				return false;
			}
			procName = getProcName(stmtType, stmtContent);
			if (!isValidName(procName)) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid procName: " + stmtContent);
				return false;
			}
			calledPair = make_pair(currProcID, procName);
			calledProcList.push_back(calledPair);
			break;


		case whileStmt:
			if (!isValidName(getControlVarName(stmtType, stmtContent))) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid control variable: " + stmtContent);
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;


		case ifStmt:
			if (!isValidName(getControlVarName(stmtType, stmtContent))) {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid control variable: " + stmtContent);
				return false;
			}
			// update if stack if it is empty, or it means if without else
			if (ifStack.empty()) {
				ifStack.push(*it);
			}
			else {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "an if without else before " + stmtContent);
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;

		case elseStmt:
			//pop only when there is one in the if stack, or it means else without if
			if (ifStack.size() == 1) {
				(*it).setNumber(ifStack.top().getNumber());
				(*it).setContent(ifStack.top().getContent());
				(*it).setType(ifStack.top().getType());
				ifStack.pop();
			}
			else {
				SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "no if before this else");
				return false;
			}
			if (!processNestedStmt(it, sourceList, brackets, calledProcList, currProcID, bracketList)) {
				return false;
			}
			break;

		default:
			SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "invalid stmt: " + stmtContent);
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
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "extra if");
		return false;
	}

	return true;
}


bool Checker::isAssignmentValid(string assignStmt) {
	//trim assignment up to ;
	size_t t = assignStmt.find(";");
	string trimmedStmt = assignStmt.substr(0, t);

	vector<string> tokens = parseExpression(trimmedStmt);
	string LHS = tokens.at(0);
	
	if (!isValidName(LHS)) {
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "LHS of expression is not a variable: "+ assignStmt);
		return false;
	}

	if (isOperator(tokens.at(2)) || isOperator(tokens.back())) {
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "expression starts or ends with an operator: " + assignStmt);
		return false;
	}

	string prevToken = "dummy";
	for (int i = 2; i < tokens.size(); ++i) {
		if (!isOperator(tokens.at(i)) && !isValidName(tokens.at(i)) && 
			!isParenthesis(tokens.at(i)) && !isConstant(tokens.at(i))) {
			SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "most probably, this case is invalid variable name at RHS of expression: "
				+ assignStmt + "\nvariable name is " + tokens.at(i));
			return false;
		}

		if (isOperator(prevToken) && isOperator(tokens.at(i))) {
			SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "two consecutive operators in an assignment: " + assignStmt);
			return false;
		}
		prevToken = tokens.at(i);
	}

	if (countNumOfLeftParenthesis(assignStmt) != countNumOfRightParenthesis(assignStmt)) {
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "unmatch of (): " + assignStmt);
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

//input will not have ;
vector<string> Checker::parseExpression(string expression) {
	size_t found = expression.find_first_of("(=+-*)");
	vector<string> result;
	string temp;

	while (found != string::npos) {
		temp = expression.substr(0, found);
		if (temp != "") {
			result.push_back(temp);
		}

		temp = expression.at(found);
		result.push_back(temp);

		expression = expression.substr(found + 1);
		found = expression.find_first_of("=+-*()");
	}

	if (!expression.empty()) {
		result.push_back(expression);
	}

	return result;
}

bool Checker::isParenthesis(string str) {
	return str == "(" || str == ")";
}

bool Checker::isOperator(string str) {
	return str == "+" || str == "-" || str == "*";
}