#include "Parser.h"


// constructor
Parser::Parser() {
}

string Parser::removeComment(string  prgm) {
	int n = prgm.length();
	string res;
	bool s_cmt = false;
	bool m_cmt = false;

	for (int i = 0; i < n; i++)
	{
		
		if (s_cmt == true && prgm[i] == '\n')
			s_cmt = false;

		else if (m_cmt == true && prgm[i] == '*' && prgm[i + 1] == '/')
			m_cmt = false, i++;

		else if (s_cmt || m_cmt)
			continue;

		else if (prgm[i] == '/' && prgm[i + 1] == '/')
			s_cmt = true, i++;
		else if (prgm[i] == '/' && prgm[i + 1] == '*')
			m_cmt = true, i++;

		else  res += prgm[i];
	}
	return res;
}

list<Statement> Parser::prepareSourceList(string source) {
	source = removeComment(source);
	addNewLineBeforeKeywords(source, " call ");
	addNewLineBeforeKeywords(source, "\tcall ");
	addNewLineBeforeKeywords(source, " procedure ");
	addNewLineBeforeKeywords(source, "\tprocedure ");
	addNewLineBeforeKeywords(source, " while ");
	addNewLineBeforeKeywords(source, "\twhile ");
	addNewLineBeforeKeywords(source, " if ");
	addNewLineBeforeKeywords(source, "\tif ");
	addNewLineBeforeKeywords(source, "else{");
	addNewLineBeforeKeywords(source, "else {");

	trim(source);
	addNewLineString(source);
	list<Statement> sourceCodeList;
	buildSourceCodeList(source, sourceCodeList);

	return sourceCodeList;
}

void Parser::parseSource(list<Statement> sourceCodeList) {
	processSourceCodeList(sourceCodeList);
	PKB::getPKBInstance()->houseKeeping();
}

void Parser::buildCFG(list<Statement> sourceCodeList) {
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

void Parser::buildSourceCodeList(string content, list<Statement>& list) {
	size_t tail = content.find('@');
	size_t head = 0;
	size_t length;
	int count = 1;

	while (tail != string::npos) {
		length = tail - head;
		string stmtContent = content.substr(head, length);
		StatementType stmtType = getTypeOfStmt(stmtContent);
		int stmtNo;

		if (stmtType == procDeclarationStmt || stmtType == elseStmt 
			|| stmtType == invalidStmt) {
			stmtNo = -1;
		}
		else {
			stmtNo = count;
			count++;
		}

		Statement stmt = Statement(stmtNo, stmtContent, stmtType);
		list.push_back(stmt);
		head = tail + 1;
		tail = content.find('@', head);
	}
}

void Parser::processSourceCodeList(list<Statement> stmtList) {
	list<int> modifiesList;
	list<int> usesList;
	list<int> childrenList;
	int currentProcID;
	list<stack<string>> bracesList;
	int prevStmtNo = -1;
	StatementType prevStmtType = StatementType::invalidStmt;

	for (list<Statement>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		int stmtNo = (*it).getNumber();
		string stmtContent = (*it).getContent();
		StatementType stmtType = (*it).getType();
		modifiesList.clear();
		usesList.clear();
		childrenList.clear();
		PKB::getPKBInstance()->addStmtToList(stmtNo, stmtType);

		switch (stmtType) {
		case assignmentStmt:
			processAssignment(stmtContent, modifiesList, usesList);
			processPatternStmt(stmtNo, stmtContent, stmtType);
			break;

		case procDeclarationStmt:
			currentProcID = PKB::getPKBInstance()->getProcID(getProcName(stmtType, stmtContent));
			break;

		case procCallStmt: 
			processCallStmt(stmtNo, stmtType, stmtContent, modifiesList, usesList);
			break;

		case whileStmt:
			processPatternStmt(stmtNo, stmtContent, stmtType);
			processNestedStmt(it, stmtList, modifiesList, usesList, childrenList, currentProcID, bracesList);
			break;

		case ifStmt: 
			processPatternStmt(stmtNo, stmtContent, stmtType);
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

void Parser::processNestedStmt(list<Statement>::iterator& it, list<Statement> stmtList,
	list<int>& modifiesList, list<int>& usesList, list<int>& childrenList, int currentProcID,
	list<stack<string>>& braceList) {
	stack <string> braces;
	braces.push("{");
	braceList.push_back(braces);

	list<int> tempModifiesList;
	list<int> tempUsesList;
	list<int> tempChildrenList;
	int prevStmtNo = -1;
	StatementType prevStmtType = StatementType::invalidStmt;

	//put control variable into UsesList
	int controlVarID = PKB::getPKBInstance()->insertVar(getControlVarName((*it).getType(), (*it).getContent()));
	usesList.push_back(controlVarID);

	++it;//to skip the starting of this while/if statement
	while (!braceList.back().empty()) {
		int stmtNo = (*it).getNumber();
		string stmtContent = (*it).getContent();
		StatementType stmtType = (*it).getType();
		tempModifiesList.clear();
		tempUsesList.clear();
		tempChildrenList.clear();
		PKB::getPKBInstance()->addStmtToList(stmtNo, stmtType);

		for (int i = 0; i < countNumOfLeftBraces(stmtContent); ++i) {
			for (list<stack<string>>::iterator iter = braceList.begin(); iter != braceList.end(); ++iter) {
				if (!(*iter).empty()) {
					(*iter).push("{");
				}
			}
		}
		for (int i = 0; i < countNumOfRightBraces(stmtContent); ++i) {
			for (list<stack<string>>::iterator iter1 = braceList.begin(); iter1 != braceList.end(); ++iter1) {
				if (!(*iter1).empty()) {
					(*iter1).pop();
				}
			}
		}

		switch (stmtType) {
		case assignmentStmt:
			processAssignment(stmtContent, tempModifiesList, tempUsesList);
			processPatternStmt(stmtNo, stmtContent, stmtType);
			break;

		case procCallStmt: 
			processCallStmt(stmtNo, stmtType, stmtContent, tempModifiesList, tempUsesList);
			break;

		case whileStmt:
			processPatternStmt(stmtNo, stmtContent, stmtType);
			processNestedStmt(it, stmtList, tempModifiesList, tempUsesList, tempChildrenList, currentProcID, braceList);
			break;

		case ifStmt:
			processPatternStmt(stmtNo, stmtContent, stmtType);
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

//input has ;, but it does not matter, cause only variables are returned
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

void Parser::processPatternStmt(int stmtNo, string stmtContent, StatementType stmtType) {
	string controlVar;
	if (stmtType == assignmentStmt) {
		size_t i = stmtContent.find("=");
		size_t j = stmtContent.find(";");
		PKB::getPKBInstance()->setPattern(stmtNo, stmtContent.substr(0, i), stmtContent.substr(i + 1, j - i - 1));
	}
	else if(stmtType == whileStmt) {
		controlVar = getControlVarName(stmtType, stmtContent);
		PKB::getPKBInstance()->setPattern(stmtNo, controlVar, "_while_");
	}
	else if (stmtType == ifStmt) {
		controlVar = getControlVarName(stmtType, stmtContent);
		PKB::getPKBInstance()->setPattern(stmtNo, controlVar, "_if_");
	}
}

void Parser::processCallStmt(int stmtNo, StatementType stmtType, string stmtContent,
	list<int>& modifiesList, list<int>& usesLise) {
	int calledProcID = PKB::getPKBInstance()->getProcID(getProcName(stmtType, stmtContent));
	modifiesList.push_back(calledProcID);
	usesLise.push_back(calledProcID);
	PKB::getPKBInstance()->addToCallStmtProcMap(stmtNo, calledProcID);
}

string Parser::getControlVarName(StatementType stmtType, string stmtContent) {
	size_t i;
	if (stmtType == whileStmt) {
		i = stmtContent.find("{");
		return stmtContent.substr(5, i - 5);
	}
	//indicate it is an ifStmt
	else {
		i = stmtContent.find("{");
		return stmtContent.substr(2, i - 6);
	}
}

string Parser::getProcName(StatementType stmtType, string stmtContent) {
	size_t i;
	if (stmtType == procCallStmt) {
		i = stmtContent.find(";");
		return stmtContent.substr(4, i - 4);
	}
	//indicate it is a procDeclarationStmt
	else {
		i = stmtContent.find("{");
		return stmtContent.substr(9, i - 9);
	}
}

StatementType Parser::getTypeOfStmt(string str) {
	size_t i, j;

	if (count(str.begin(), str.end(), '=') == 1) {
		i = str.find(";");
		j = str.find("=");
		if (isValidName(str.substr(0, j))
			&& isAllClosingBraces(str.substr(i + 1))) {
			return StatementType::assignmentStmt;
		}
		else {
			return StatementType::invalidStmt;
		}
	}

	if (str.substr(0, 9) == "procedure") {
		i = str.find("{");
		if (isValidName(str.substr(9, i - 9))) {
			return StatementType::procDeclarationStmt;
		}
		else {
			return StatementType::invalidStmt;
		}
	}

	if (str.substr(0, 4) == "call") {
		i = str.find(";");
		if (isValidName(str.substr(4, i - 4)) &&
			isAllClosingBraces(str.substr(i + 1))) {
			return StatementType::procCallStmt;
		}
		else {
			return StatementType::invalidStmt;
		}
	}

	if (str.substr(0, 5) == "while") {
		i = str.find("{");
		if (isValidName(str.substr(5, i - 5))) {
			return StatementType::whileStmt;
		}
		else {
			return StatementType::invalidStmt;
		}
	}

	if (str.substr(0, 2) == "if") {
		i = str.find("{");
		if (i > 6 && str.substr(i - 4, 4) == "then" && isValidName(str.substr(2, i - 6))) {
			return StatementType::ifStmt;
		}
		else {
			return StatementType::invalidStmt;
		}
	}

	if (str == "else{") {
		return StatementType::elseStmt;
	}

	return StatementType::invalidStmt;
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