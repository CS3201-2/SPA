#include "Parser.h"

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
	addNewLineBeforeKeywords(source, " call ");
	addNewLineBeforeKeywords(source, "\tcall ");
	addNewLineBeforeKeywords(source, " procedure ");
	addNewLineBeforeKeywords(source, "\tprocedure ");
	addNewLineBeforeKeywords(source, " while ");
	addNewLineBeforeKeywords(source, "\twhile ");
	addNewLineBeforeKeywords(source, " if ");
	addNewLineBeforeKeywords(source, "\tif ");
	addNewLineBeforeKeywords(source, "else{");
	
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

void Parser::addNewLineBeforeKeywords(string& source, string keywords) {
	size_t found = source.find(keywords);
	while (found != string::npos) {
		source.insert(found, "@");
		found = source.find(keywords, found + 2);
	}
}

void Parser::trim(string& line) {
	line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
}

void Parser::addNewLineString(string &content) {
	size_t index = content.find(';');

	while (index != string::npos) {
		index++;
		while (index != content.size() && content.at(index) == '}') index++;
		if (index < content.size() && content.at(index) != '@') {
			content.insert(index, (size_t)1, '@');
		}
		index = content.find(';', index);
	}
	index = content.find('{');
	while (index != string::npos) {
		index++;
		if (index < content.size() && content.at(index) != '@') {
			content.insert(index, (size_t)1, '@');
		}
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

		if (getTypeOfStmt(line) == procDeclarationStmt ||
			getTypeOfStmt(line) == elseStmt || getTypeOfStmt(line) == invalidStmt) {
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
			&& prevStmtNo != stmtNo && prevStmtType != invalidStmt) {
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
			&& prevStmtNo != stmtNo && prevStmtType != invalidStmt) {
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
	list<string> varList = parseExpression(str);
	
	for (list<string>::iterator it = varList.begin(); it != varList.end(); ++it) {
		if (isValidName(*it)) {
			int varID = PKB::getPKBInstance()->insertVar(*it);

			if (modifiesList.empty()) {
				modifiesList.push_back(varID);
			}
			else {
				usesList.push_back(varID);
			}
		}
		else if(isConstant(*it)) {
			PKB::getPKBInstance()->addConstantToList(stoi(*it));
		}
	}
}

list<string> Parser::parseExpression(string expression) {
	size_t found = expression.find_first_of("(=+-*);");
	list<string> result;
	string temp;

	while (found != string::npos) {
		temp = expression.substr(0, found);
		if (temp != "") {
			result.push_back(temp);
		}

		expression = expression.substr(found + 1);
		found = expression.find_first_of("=+-*();");
	}

	return result;
}

void Parser::processPatternStmt(int stmtNo, string stmt, int stmtType) {
	string controlVar;
	if (stmtType == assignmentStmt) {
		size_t i = stmt.find("=");
		size_t j = stmt.find(";");
		PKB::getPKBInstance()->setPattern(stmtNo, stmt.substr(0, i), stmt.substr(i + 1, j - i - i));
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

string Parser::getProcName(int stmtType, string stmt) {
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

int Parser::getTypeOfStmt(string str) {
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

bool Parser::isConstant(string str) {
	for (char& c : str) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

bool Parser::isValidName(string str) {
	if (!isalpha(str.at(0))) {
		return false;
	}

	for (char& c : str) {
		if (!isalnum(c)) {
			return false;
		}
	}

	return true;
}

bool Parser::isAllClosingBraces(string str) {
	if (!str.empty()) {
		return str.find_first_not_of("}") == string::npos;
	}

	return true;
}