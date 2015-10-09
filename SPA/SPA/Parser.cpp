#include "Parser.h"


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

// constructor
Parser::Parser() {
}

list<pair<int, string>> Parser::prepareSourceList(string source) {
	trim(source);
	addNewLineString(source);
	list<pair<int, string>> sourceCodeList;
	buildSourceCodeList(source, sourceCodeList);

	return sourceCodeList;
}

void Parser::parseSource(list<pair<int, string>> sourceCodeList) {
	processSourceCodeList(sourceCodeList);
	PKB::getPKBInstance()->houseKeeping();
}

void Parser::buildCFG(list<pair<int, string>> sourceCodeList) {
	PKB::getPKBInstance()->buildCFG(sourceCodeList);
}

void Parser::trim(string& line) {
	line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
}

void Parser::addNewLineString(string &content) {
	size_t index = content.find(';');

	while (index != string::npos) {
		index++;
		while (index != content.size() && content.at(index) == '}') index++;
		content.insert(index, (size_t)1, '@');
		index = content.find(';', index);
	}
	index = content.find('{');
	while (index != string::npos) {
		index++;
		content.insert(index, (size_t)1, '@');
		index = content.find('{', index);
	}
	
	if (content.back() != '@') {
		content += "@";
	}
}

void Parser::buildSourceCodeList(string content, list<pair<int, string>>& list) {
	size_t tail = content.find('@');
	size_t head = 0;
	size_t length;
	int count = 1;
	string line;

	while (tail != string::npos) {
		length = tail - head;
		line = content.substr(head, length);
		pair<int, string> pair;

		if (regex_match(line, procDeclarationRegex) || regex_match(line, elseRegex)) {
			pair.first = -1;
		}
		else {
			pair.first = count;
			count++;
		}
		pair.second = line;
		list.push_back(pair);
		head = tail + 1;
		tail = content.find('@', head);
	}
}

void Parser::processSourceCodeList(list<pair<int, string>>& stmtList) {
	list<int> modifiesList;
	list<int> usesList;
	list<int> childrenList;
	int currentProcID;
	list<stack<string>> bracesList;
	int prevStmtNo = -1;
	int prevStmtType = invalidStmt;

	for (list<pair<int, string>>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		int stmtNo = (*it).first;
		string stmt = (*it).second;
		int stmtType = getTypeOfStmt(stmt);
		modifiesList.clear();
		usesList.clear();
		childrenList.clear();
		PKB::getPKBInstance()->addStmtToList(stmtNo, stmtType);

		switch (stmtType) {
		case assignmentStmt:
			processAssignment(stmt, modifiesList, usesList);
			processPatternStmt(stmtNo, stmt, stmtType);
			break;

		case procDeclarationStmt:
			currentProcID = PKB::getPKBInstance()->getProcID(getProcName(stmtType, stmt));
			break;

		case procCallStmt: 
			processCallStmt(stmtNo, stmtType, stmt, modifiesList, usesList);
			break;

		case whileStmt:
			processPatternStmt(stmtNo, stmt, stmtType);
			processNestedStmt(it, stmtList, modifiesList, usesList, childrenList, currentProcID, bracesList);
			break;

		case ifStmt: 
			processPatternStmt(stmtNo, stmt, stmtType);
			processNestedStmt(it, stmtList, modifiesList, usesList, childrenList, currentProcID, bracesList);
			break;
		default: break;//should never reach this part
		}

		//modifies
		PKB::getPKBInstance()->setModifies(stmtNo, modifiesList);
		PKB::getPKBInstance()->setModifies(currentProcID, modifiesList);
		
		//uses
		PKB::getPKBInstance()->setUses(stmtNo, usesList);
		PKB::getPKBInstance()->setUses(currentProcID, usesList);
		
		//follows
		if (prevStmtType != procDeclarationStmt && stmtType != procDeclarationStmt 
			&& prevStmtNo != stmtNo) {
			PKB::getPKBInstance()->setFollows(prevStmtNo, stmtNo);
		}
		prevStmtNo = stmtNo;
		prevStmtType = stmtType;
		
		//parent
		if (stmtType == whileStmt || stmtType == ifStmt) {
			PKB::getPKBInstance()->setParent(stmtNo, childrenList);
		}
	}
}

int Parser::countNumOfLeftBraces(string str) {
	return count(str.begin(), str.end(), '{');
}

int Parser::countNumOfRightBraces(string str) {
	return count(str.begin(), str.end(), '}');
}

void Parser::processNestedStmt(list<pair<int, string>>::iterator& it, list<pair<int, string>>& stmtList,
	list<int>& modifiesList, list<int>& usesList, list<int>& childrenList, int currentProcID,
	list<stack<string>>& braceList) {
	stack <string> braces;
	braces.push("{");
	braceList.push_back(braces);

	list<int> tempModifiesList;
	list<int> tempUsesList;
	list<int> tempChildrenList;
	int prevStmtNo = -1;
	int prevStmtType = invalidStmt;

	//put control variable into UsesList
	int controlVarID = PKB::getPKBInstance()->insertVar(getControlVarName(getTypeOfStmt((*it).second), (*it).second));
	usesList.push_back(controlVarID);

	++it;//to skip the starting of this while/if statement
	while (!braceList.back().empty()) {
		int stmtNo = (*it).first;
		string stmt = (*it).second;
		int stmtType = getTypeOfStmt(stmt);
		tempModifiesList.clear();
		tempUsesList.clear();
		tempChildrenList.clear();
		PKB::getPKBInstance()->addStmtToList(stmtNo, stmtType);

		for (int i = 0; i < countNumOfLeftBraces(stmt); ++i) {
			for (list<stack<string>>::iterator iter = braceList.begin(); iter != braceList.end(); ++iter) {
				if (!(*iter).empty()) {
					(*iter).push("{");
				}
			}
		}
		for (int i = 0; i < countNumOfRightBraces(stmt); ++i) {
			for (list<stack<string>>::iterator iter1 = braceList.begin(); iter1 != braceList.end(); ++iter1) {
				if (!(*iter1).empty()) {
					(*iter1).pop();
				}
			}
		}

		switch (stmtType) {
		case assignmentStmt:
			processAssignment(stmt, tempModifiesList, tempUsesList);
			processPatternStmt(stmtNo, stmt, stmtType);
			break;

		case procCallStmt: 
			processCallStmt(stmtNo, stmtType, stmt, tempModifiesList, tempUsesList);
			break;

		case whileStmt:
			processPatternStmt(stmtNo, stmt, stmtType);
			processNestedStmt(it, stmtList, tempModifiesList, tempUsesList, tempChildrenList, currentProcID, braceList);
			break;

		case ifStmt:
			processPatternStmt(stmtNo, stmt, stmtType);
			processNestedStmt(it, stmtList, tempModifiesList, tempUsesList, tempChildrenList, currentProcID, braceList);
			break;
		}

		if (stmtNo != -1) {
			childrenList.push_back(stmtNo);
		}

		//modifies
		PKB::getPKBInstance()->setModifies(stmtNo, tempModifiesList);
		PKB::getPKBInstance()->setModifies(currentProcID, tempModifiesList);
		
		//uses
		PKB::getPKBInstance()->setUses(stmtNo, tempUsesList);
		PKB::getPKBInstance()->setUses(currentProcID, tempUsesList);

		//follows
		if (prevStmtType != procDeclarationStmt && stmtType != procDeclarationStmt 
			&& prevStmtNo != stmtNo) {
			PKB::getPKBInstance()->setFollows(prevStmtNo, stmtNo);
		}
		prevStmtNo = stmtNo;
		prevStmtType = stmtType;

		//parent
		if (stmtType == whileStmt || stmtType == ifStmt) {
			PKB::getPKBInstance()->setParent(stmtNo, tempChildrenList);
		}

		modifiesList.insert(modifiesList.end(), tempModifiesList.begin(), tempModifiesList.end());
		usesList.insert(usesList.end(), tempUsesList.begin(), tempUsesList.end());

		if (!braceList.back().empty()) {
			++it;
		}
	}
	braceList.pop_back();
}

void Parser::processAssignment(string str, list<int>& modifiesList, list<int>& usesList) {
	string variable = "";

	for (string::iterator it = str.begin(); it != str.end(); ++it) {
		if (isMathSymbol(*it) || isSemicolon(*it)) {
			if (isVariable(variable)) {
				int varID = PKB::getPKBInstance()->insertVar(variable);
				
				if (modifiesList.empty()) {
					modifiesList.push_back(varID);
				}
				else {
					usesList.push_back(varID);
				}
			}
			if (isConstant(variable)) {
				PKB::getPKBInstance()->addConstantToList(stoi(variable));
			}

			variable = "";
		}
		else {
			variable += *it;
		}
	}
}

bool Parser::isVariable(string str) {
	return regex_match(str, variableRegex);
}

bool Parser::isSemicolon(char ch) {
	return ch == ';';
}

//test whether a char is + - *
bool Parser::isMathSymbol(char ch) {
	switch (ch) {
	case '+': case'-': case'*': case'=':
		return true;
	default:
		return false;
	}
}

int Parser::getTypeOfStmt(string str) {
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

void Parser::processPatternStmt(int stmtNo, string stmt, int stmtType) {
	smatch m;
	string controlVar;
	if (stmtType == assignmentStmt) {
		regex_search(stmt, m, assignmentRegex);
		//1 is the LHS of assignment, 4 is the RHS of assignment
		PKB::getPKBInstance()->setPattern(stmtNo, m[1], m[4]);
	}
	else if(stmtType == whileStmt) {
		controlVar = getControlVarName(stmtType, stmt);
		PKB::getPKBInstance()->setPattern(stmtNo, controlVar, "_while_");
	}
	else if (stmtType == ifStmt) {
		controlVar = getControlVarName(stmtType, stmt);
		PKB::getPKBInstance()->setPattern(stmtNo, controlVar, "_if_");
	}
}

void Parser::processCallStmt(int stmtNo, int stmtType, string stmt,
	list<int>& modifiesList, list<int>& usesLise) {
	int calledProcID = PKB::getPKBInstance()->getProcID(getProcName(stmtType, stmt));
	modifiesList.push_back(calledProcID);
	usesLise.push_back(calledProcID);
	PKB::getPKBInstance()->addToCallStmtProcMap(stmtNo, calledProcID);
}

string Parser::getControlVarName(int stmtType, string stmt) {
	smatch m;
	if (stmtType == whileStmt) {
		regex_search(stmt, m, whileRegex);
		return m[1];
	}
	else {
		regex_search(stmt, m, ifRegex);
		return m[1];
	}
}

string Parser::getProcName(int stmtType, string str) {
	smatch m;
	if (stmtType == procCallStmt) {
		regex_search(str, m, procCallRegex);
		return m[1];
	}
	else {
		regex_search(str, m, procDeclarationRegex);
		return m[1];
	}
}

bool Parser::isConstant(string str) {
	size_t found = str.find_first_not_of("1234567890");
	return (found == string::npos);
}