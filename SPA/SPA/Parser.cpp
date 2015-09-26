#include "Parser.h"
#include "PKB.h"
#include "AST.h"
#include <string>
#include <list>
#include <algorithm>
#include <regex>

const regex assignmentRegex("((^[[:alpha:]])([[:alnum:]]+)*)=(.*);\\}*");
const regex procDeclarationRegex("procedure((^[[:alpha:]])([[:alnum:]]+)*)\\{");
const regex procCallRegex("call((^[[:alpha:]])([[:alnum:]]+)*);\\}*");
const regex whileRegex("while((^[[:alpha:]])([[:alnum:]]+)*)\\{");
const regex ifRegex("if((^[[:alpha:]])([[:alnum:]]+)*)then\\{");
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

PKB Parser::parseSource( string source ) {
	
	trim(source);

	addNewLineString(source);
	
	list<std::pair<int, string>> sourceCodeList;

	buildSourceCodeList(source, sourceCodeList);
	
	processSourceCodeList(sourceCodeList);

	return pkb;
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
	
	cout << content << endl;

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
	regex procDeclaration("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{");
	regex elseStmt("else\\{");

	while (tail != std::string::npos) {
		length = tail - head;
		line = content.substr(head, length);
		std::pair<int, string> pair;

		if (regex_match(line, procDeclaration) || regex_match(line, elseStmt)) {
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
	int currentProcedureID;
	list<stack<string>> bracesList;
	int prevStmtLine = -1;
	int prevStmtType = invalidStmt;

	for (list<std::pair<int, string>>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		int stmtNumber = (*it).first;
		int stmtType = getTypeOfStatement((*it).second);

		switch (stmtType) {
		case assignmentStmt:
			pkb.addAssignList((*it).first);
			processAssignment(*it, modifiesList, usesList);
			processPatternStmt(*it, stmtType);
			break;
		case procDeclarationStmt:
			pkb.getProcTable().insertProc(getProcName((*it).second));
			currentProcedureID = pkb.getProcTable().getIndex(getProcName((*it).second));
			modifiesList.clear();
			usesList.clear();
			break;
		case procCallStmt: break;
		case whileStmt:
			pkb.addWhileList((*it).first);
			processWhile(it, stmtList, modifiesList, usesList, childrenList, currentProcedureID, bracesList);
			break;
		case ifStmt: break;//for if
		case elseStmt: break;//for else
		case invalidStmt: break;//for invalid statement
		default: break;
		}

		//modifies
		for (list<int>::iterator listIter1 = modifiesList.begin(); listIter1 != modifiesList.end(); ++listIter1) {
			modifies.setModifiesStmt(*listIter1, stmtNumber);
			modifies.setModifiesStmt(*listIter1, currentProcedureID);
		}
		//uses
		for (list<int>::iterator listIter2 = usesList.begin(); listIter2 != usesList.end(); ++listIter2) {
			uses.setUsesStmt(*listIter2, stmtNumber);
			uses.setUsesStmt(*listIter2, currentProcedureID);
		}
		//follows
		if ((prevStmtType != procDeclarationStmt && prevStmtType != elseStmt && prevStmtType != invalidStmt)
			&& stmtType != procDeclarationStmt) {
			follows.setFollowsStmt(prevStmtLine, stmtNumber);
		}
		prevStmtLine = stmtNumber;
		prevStmtType = stmtType;
		//parent
		if (stmtType == whileStmt) {
			parent.setParentStmt(stmtNumber, childrenList);
		}
		childrenList.empty();
	}
}

int Parser::countNumOfLeftBraces(std::pair<int,string> pair) {
	string str = pair.second;
	return std::count(str.begin(), str.end(), '{');
}

int Parser::countNumOfRightBraces(std::pair<int, string> pair) {
	string str = pair.second;
	return std::count(str.begin(), str.end(), '}');
}

void Parser::processWhile(list<pair<int, string>>::iterator& it, list<std::pair<int, string>>& stmtList,
	list<int>& modifiesList, list<int>& usesList, list<int>& childrenList, int currentProcedureID,
	list<stack<string>>& braceList) {
	stack <string> braces;
	braces.push("{");
	braceList.push_back(braces);

	modifiesList.clear();
	usesList.clear();

	list<int> tempModifiesList;
	list<int> tempUsesList;
	list<int> tempChildrenList;
	int prevStmtLine = -1;
	int prevStmtType = invalidStmt;

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
		int stmtType = getTypeOfStatement((*it).second);

		switch (stmtType) {
		case assignmentStmt:
			pkb.addAssignList((*it).first);
			processAssignment(*it, tempModifiesList, tempUsesList);
			processPatternStmt(*it, stmtType);
			break;
		case procDeclarationStmt: break; //error
		case procCallStmt: break;
		case whileStmt:
			pkb.addWhileList((*it).first);
			processWhile(it, stmtList, tempModifiesList, tempUsesList, tempChildrenList, currentProcedureID, braceList);
			break;
		case ifStmt: break;//for if
		case elseStmt: break;//for else
		case invalidStmt: break;//for invalid statement
		}
		if (stmtNumber != -1) {
			childrenList.push_back(stmtNumber);
		}

		//modifies
		for (list<int>::iterator listIter1 = tempModifiesList.begin(); listIter1 != tempModifiesList.end(); ++listIter1) {
			modifies.setModifiesStmt(*listIter1, stmtNumber);
			modifies.setModifiesStmt(*listIter1, currentProcedureID);
		}
		//uses
		for (list<int>::iterator listIter2 = tempUsesList.begin(); listIter2 != tempUsesList.end(); ++listIter2) {
			uses.setUsesStmt(*listIter2, stmtNumber);
			uses.setUsesStmt(*listIter2, currentProcedureID);
		}

		//follows
		if ((prevStmtType != procDeclarationStmt && prevStmtType != elseStmt && prevStmtType != invalidStmt)
			&& stmtType != procDeclarationStmt) {
			follows.setFollowsStmt(prevStmtLine, stmtNumber);
		}
		prevStmtLine = stmtNumber;
		prevStmtType = stmtType;

		//parent
		if (stmtType == whileStmt) {
			parent.setParentStmt(stmtNumber, tempChildrenList);
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
				VarTable& varTable = pkb.getVarTable();
				int varID = varTable.getIndex(variable);
				
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

string Parser::getProcName(string str) {
	smatch m;
	regex_search(str, m, procDeclarationRegex);
	return m[1];
}

int Parser::getTypeOfStatement(string str) {
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
	regex_search(stmt.second, m, assignmentRegex);
	//1 is the LHS of assignment, 4 is the RHS of assignment
	pattern.setPattern(stmt.first, m[1], m[4]);
}