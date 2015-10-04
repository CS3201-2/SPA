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
Parser::Parser()
{

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
	PKB::getPKBInstance()->buildCFG(sourceCodeList);
}

void Parser::trim(string& line) {
	line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
}

void Parser::addNewLineString(string &content) {
	string::iterator itr;

	size_t index = content.find(';');

	while (index != std::string::npos) {
		index++;
		while (index != content.size() && content.at(index) == '}') index++;
		content.insert(index, (size_t)1, '@');
		index = content.find(';', index);
	}
	index = content.find('{');
	while (index != std::string::npos) {
		index++;
		content.insert(index, (size_t)1, '@');
		index = content.find('{', index);
	}
	
	if (content.back() != '@') {
		content += "@";
	}
}

void Parser::buildSourceCodeList(string content, list<std::pair<int, string>>& list) {
	size_t tail = content.find('@');
	size_t head = 0;
	size_t length;
	int count = 1;
	string line;

	while (tail != std::string::npos) {
		length = tail - head;
		line = content.substr(head, length);
		std::pair<int, string> pair;

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
	//int1 is the currentProcID, pair is the *it
	pair<int, pair<int, string>> temp;
	list<pair<int, pair<int, string>>> calledProcList;
	int currentProcID;
	int calledProcID;
	list<stack<string>> bracesList;
	int prevStmtLine = -1;
	int prevStmtType = invalidStmt;

	for (list<std::pair<int, string>>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		int stmtNumber = (*it).first;
		string stmt = (*it).second;
		int stmtType = getTypeOfStmt(stmt);

		switch (stmtType) {
		case assignmentStmt:
			PKB::getPKBInstance()->addAssignToList(stmtNumber);
			processAssignment(*it, modifiesList, usesList);
			processPatternStmt(*it, stmtType);
			break;

		case procDeclarationStmt:
			//PKB::getPKBInstance()->insertProc(getProcName(stmtType, (*it).second));
			currentProcID = PKB::getPKBInstance()->insertProc(getProcName(stmtType, (*it).second));
			modifiesList.clear();
			usesList.clear();
			break;

		case procCallStmt: 
			PKB::getPKBInstance()->addCallToList(stmtNumber);
			temp = make_pair(currentProcID, *it);
			//calledProcList.push_back(temp);
			modifiesList.clear();
			usesList.clear();
			calledProcID = PKB::getPKBInstance()->getProcID(getProcNameCallStmt(stmt));
			modifiesList.push_back(calledProcID);
			usesList.push_back(calledProcID);
			break;

		case whileStmt:
			PKB::getPKBInstance()->addWhileToList(stmtNumber);
			processPatternStmt(*it, stmtType);
			//push control variable to useslist
			processNestedStmt(it, stmtList, modifiesList, usesList, childrenList, currentProcID, bracesList, calledProcList);
			break;

		case ifStmt: 
			PKB::getPKBInstance()->addIfToList(stmtNumber);
			processPatternStmt(*it, stmtType);
			//push control variable to useslist
			processNestedStmt(it, stmtList, modifiesList, usesList, childrenList, currentProcID, bracesList, calledProcList);
			break;

		case invalidStmt: break;//for invalid statement
		default: break;
		}

		//modifies
		PKB::getPKBInstance()->setModifies(stmtNumber, modifiesList);
		PKB::getPKBInstance()->setModifies(currentProcID, modifiesList);
		
		//uses
		PKB::getPKBInstance()->setUses(stmtNumber, usesList);
		PKB::getPKBInstance()->setUses(currentProcID, usesList);
		
		//follows
		if ((prevStmtType != procDeclarationStmt && prevStmtType != elseStmt && prevStmtType != invalidStmt)
			&& stmtType != procDeclarationStmt && prevStmtLine != stmtNumber) {
			PKB::getPKBInstance()->setFollows(prevStmtLine, stmtNumber);
		}
		prevStmtLine = stmtNumber;
		prevStmtType = stmtType;
		//parent
		if (stmtType == whileStmt || stmtType == ifStmt) {
			PKB::getPKBInstance()->setParent(stmtNumber, childrenList);
		}
		//process call stmt for modifies, uses, call
		//processCalledProcList(calledProcList);
	}
}

int Parser::countNumOfLeftBraces(std::pair<int,string> pair) {
	string str = pair.second;
	return count(str.begin(), str.end(), '{');
}

int Parser::countNumOfRightBraces(std::pair<int, string> pair) {
	string str = pair.second;
	return count(str.begin(), str.end(), '}');
}

void Parser::processNestedStmt(list<pair<int, string>>::iterator& it, list<std::pair<int, string>>& stmtList,
	list<int>& modifiesList, list<int>& usesList, list<int>& childrenList, int currentProcID,
	list<stack<string>>& braceList, list<pair<int,pair<int, string>>>& calledProcList) {
	stack <string> braces;
	braces.push("{");
	braceList.push_back(braces);

	modifiesList.clear();
	usesList.clear();
	childrenList.clear();

	list<int> tempModifiesList;
	list<int> tempUsesList;
	list<int> tempChildrenList;
	pair<int, pair<int, string>> temp;
	int prevStmtLine = -1;
	int prevStmtType = invalidStmt;
	int calledProcID;

	//put control variable into Uses
	int controlVarID = PKB::getPKBInstance()->insertVar(getControlVarName(getTypeOfStmt((*it).second), (*it).second));
	list<int> controlVarList;
	controlVarList.push_back(controlVarID);
	PKB::getPKBInstance()->setUses((*it).first, controlVarList);
	PKB::getPKBInstance()->setUses(currentProcID, controlVarList);

	++it;//to skip the starting of this while statement
	while (!braceList.back().empty()) {
		for (int i = 0; i < countNumOfLeftBraces(*it); ++i) {
			for (list<stack<string>>::iterator iter = braceList.begin(); iter != braceList.end(); ++iter) {
				if (!(*iter).empty()) {
					(*iter).push("{");
				}
			}
		}
		for (int i = 0; i < countNumOfRightBraces(*it); ++i) {
			for (list<stack<string>>::iterator iter1 = braceList.begin(); iter1 != braceList.end(); ++iter1) {
				if (!(*iter1).empty()) {
					(*iter1).pop();
				}
			}
		}

		int stmtNumber = (*it).first;
		string stmt = (*it).second;
		int stmtType = getTypeOfStmt(stmt);

		switch (stmtType) {
		case assignmentStmt:
			PKB::getPKBInstance()->addAssignToList(stmtNumber);
			processAssignment(*it, tempModifiesList, tempUsesList);
			processPatternStmt(*it, stmtType);
			break;

		case procCallStmt: 
			PKB::getPKBInstance()->addCallToList(stmtNumber);
			tempModifiesList.clear();
			tempUsesList.clear();
			//temp = make_pair(currentProcID, *it);
			//calledProcList.push_back(temp);
			calledProcID = PKB::getPKBInstance()->getProcID(getProcNameCallStmt(stmt));
			tempModifiesList.push_back(calledProcID);
			tempUsesList.push_back(calledProcID);
			break;

		case whileStmt:
			PKB::getPKBInstance()->addWhileToList(stmtNumber);
			processPatternStmt(*it, stmtType);
			processNestedStmt(it, stmtList, tempModifiesList, tempUsesList, tempChildrenList, currentProcID, braceList, calledProcList);
			break;

		case ifStmt:
			PKB::getPKBInstance()->addIfToList(stmtNumber);
			processPatternStmt(*it, stmtType);
			processNestedStmt(it, stmtList, tempModifiesList, tempUsesList, tempChildrenList, currentProcID, braceList, calledProcList);
			break;//for if

		case invalidStmt: break;//for invalid statement
		}
		if (stmtNumber != -1) {
			childrenList.push_back(stmtNumber);
		}

		//modifies
		PKB::getPKBInstance()->setModifies(stmtNumber, tempModifiesList);
		PKB::getPKBInstance()->setModifies(currentProcID, tempModifiesList);
		
		//uses
		PKB::getPKBInstance()->setUses(stmtNumber, tempUsesList);
		PKB::getPKBInstance()->setUses(currentProcID, tempUsesList);

		//follows
		if ((prevStmtType != procDeclarationStmt && prevStmtType != elseStmt && prevStmtType != invalidStmt)
			&& stmtType != procDeclarationStmt && prevStmtLine != stmtNumber) {
			PKB::getPKBInstance()->setFollows(prevStmtLine, stmtNumber);
		}
		prevStmtLine = stmtNumber;
		prevStmtType = stmtType;

		//parent
		if (stmtType == whileStmt || stmtType == ifStmt) {
			PKB::getPKBInstance()->setParent(stmtNumber, tempChildrenList);
		}

		modifiesList.splice(modifiesList.end(), tempModifiesList);
		usesList.splice(usesList.end(), tempUsesList);

		if (!braceList.back().empty()) {
			++it;
		}
	}
	braceList.pop_back();
}

void Parser::processAssignment(std::pair<int,string> pair, list<int>& modifiesList, list<int>& usesList) {
	string str = pair.second;
	int stmtNumber = pair.first;
	string::iterator it;
	string variable = "";

	modifiesList.clear();
	usesList.clear();

	for (it = str.begin(); it != str.end(); ++it) {
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
	if (ch == ';') {
		return true;
	}
	else {
		return false;
	}
}

//test whether a char is + - * /
bool Parser::isMathSymbol(char ch) {
	switch (ch) {
	case '+': case'-': case'*': case'=':
		return true;
	default:
		return false;
	}
}

string Parser::getProcName(int stmtType, string str) {
	smatch m;
	if (stmtType == procDeclarationStmt) {
		regex_search(str, m, procDeclarationRegex);
	}
	else {
		regex_search(str, m, procCallRegex);
	}
	return m[1];
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

void Parser::processPatternStmt(pair<int, string> stmt, int stmtType) {
	//currently it is only for assignment
	smatch m;
	string controlVar;
	if (stmtType == assignmentStmt) {
		regex_search(stmt.second, m, assignmentRegex);
		//1 is the LHS of assignment, 4 is the RHS of assignment
		PKB::getPKBInstance()->setPattern(stmt.first, m[1], m[4]);
	}
	else if(stmtType == whileStmt) {
		controlVar = getControlVarName(stmtType, stmt.second);
		PKB::getPKBInstance()->setPattern(stmt.first, controlVar, "_while_");
	}
	else if (stmtType == ifStmt) {
		controlVar = getControlVarName(stmtType, stmt.second);
		PKB::getPKBInstance()->setPattern(stmt.first, controlVar, "_if_");
	}
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

string Parser::getProcNameCallStmt(string str) {
	smatch m;
	regex_search(str, m, procCallRegex);
	return m[1];
}