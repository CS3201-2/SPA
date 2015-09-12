#include "Parser.h"
#include "PKB.h"
#include "AST.h"
#include <string>
#include <list>
#include <algorithm>
#include <regex>

const regex assignmentRegex("(([[:alpha:]])([[:alnum:]]+)*)=(.*);\\}*");
const regex procDeclarationRegex("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{");
const regex procCallRegex("call(([[:alpha:]])([[:alnum:]]+)*);\\}*");
const regex whileRegex("while(([[:alpha:]])([[:alnum:]]+)*)\\{");
const regex ifRegex("if(([[:alpha:]])([[:alnum:]]+)*)then\\{");
const regex elseRegex("else\\{");
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

PKB Parser::parseSource( string source ) {
	
	trim(source);

	addNewLineString(source);
	
	list<std::pair<int, string>> sourceCodeList;

	buildSourceCodeList(source, sourceCodeList);
	
	processSourceCodeList(sourceCodeList);
	cout << "end of everything before ast." << endl;
	AST ast = AST();
	ast.acceptStatements(sourceCodeList);
	pkb.setAST(ast);

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
		while (index != content.size() - 1 && content.at(index) == '}') index++;
		content.insert(index, (size_t)1, '@');
		index = content.find(';', index);
	}

	index = content.find('{');
	while (index != std::string::npos) {
		index++;
		content.insert(index, (size_t)1, '@');
		index = content.find('{', index);
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

void Parser::processSourceCodeList(list<std::pair<int, string>>& stmtList) {
	list<int> modifiesList;
	list<int> usesList;

	Modifies& modifies = pkb.getModifies();
	Uses& uses = pkb.getUses();
	for (list<std::pair<int,string>>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		switch (getTypeOfStatement(*it)){
		case assignmentStmt: 
			pkb.addAssignList((*it).first); 
			processAssignment(*it, modifiesList, usesList); 
			break;
		case procDeclarationStmt: 
			modifiesList.clear();
			usesList.clear();
			break;
		case procCallStmt: break;
		case whileStmt: 
			pkb.addWhileList((*it).first); 
			processWhile(it, stmtList, modifiesList, usesList); 
			break;
		case ifStmt: break;//for if
		case elseStmt: break;//for else
		case invalidStmt: break;//for invalid statement
		default: break;
		}
		
		int stmtNumber = (*it).first;
		for (list<int>::iterator listIter1 = modifiesList.begin(); listIter1 != modifiesList.end(); ++listIter1) {
			modifies.set_modifies_stmt(*listIter1, stmtNumber);
		}
		for (list<int>::iterator listIter2 = usesList.begin(); listIter2 != usesList.end(); ++listIter2) {
			uses.set_uses_stmt(*listIter2, stmtNumber);
		}
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

void Parser::processWhile(list<pair<int, string>>::iterator it, list<std::pair<int, string>>& stmtList, list<int>& modifiesList, list<int>& usesList) {
	stack <string> braces;
	braces.push("{");

	modifiesList.clear();
	usesList.clear();

	list<int> tempModifiesList;
	list<int> tempUsesList;

	++it;//to skip the starting of this while statement
	while (!braces.empty()) {
		for (int i = 0; i < countNumOfLeftBraces(*it); ++i) {
			braces.push("{");
		}
		for (int i = 0; i < countNumOfRightBraces(*it); ++i) {
			if (!braces.empty()) {
				braces.pop();
			}
		}
		switch (getTypeOfStatement(*it)) {
		case assignmentStmt: pkb.addAssignList((*it).first); processAssignment(*it, tempModifiesList, tempUsesList); break;
		case procDeclarationStmt: break;
		case procCallStmt: break;
		case whileStmt: pkb.addWhileList((*it).first); processWhile(it, stmtList, tempModifiesList, tempUsesList); break;
		case ifStmt: break;//for if
		case elseStmt: break;//for else
		case invalidStmt: break;//for invalid statement
		}

		modifiesList.splice(modifiesList.end(), tempModifiesList);
		usesList.splice(usesList.end(), tempUsesList);

		++it;
	}
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
				int varID = varTable.get_ID(variable);
				
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
	regex variable("(^[[:alpha:]])([[:alnum:]]+)*$");
	return regex_match(str, variable);
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
	case '+': case'-': case'*': case'/': case'=':
		return true;
	default:
		return false;
	}
}

int Parser::getTypeOfStatement(pair<int, string> pair) {


	string str = pair.second;

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