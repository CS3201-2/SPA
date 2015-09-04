#include "Parser.h"
#include "PKB.h"
#include <string>
#include <list>
#include <algorithm>
#include <regex>

// constructor
Parser::Parser()
{

}

PKB Parser::parseSource( string source ) {
	
	trim(source);

	addNewLineString(source);
	
	list<string> sourceCodeList;

	buildSourceCodeList(source, sourceCodeList);

	// comments for Macong: sourceCodeList is the list filled with SOURCE line strings

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
		if (content.at(index) == '}') index++;
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

void Parser::buildSourceCodeList(string content, list<string>& list) {

	size_t tail = content.find('@');
	size_t head = 0;
	size_t length;
	while (tail != std::string::npos) {
		length = tail - head;
		list.push_back(content.substr(head, length));
		head = tail + 1;
		tail = content.find('@', head);
	}
}

void Parser::processSourceCodeList(list<string>& stmtList) {
	list<int> modifiesList;
	list<int> usesList;

	for (list<string>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		switch (getTypeOfStatement(*it)) {
		case 0: processAssignment(*it, modifiesList, usesList); break;
		case 1: break;
		case 2: break;
		case 3: processWhile(it, stmtList, modifiesList, usesList); break;
		case 4: break;//for if
		case 5: break;//for else
		case 6: break;//for invalid statement
		default: break;
		}

		modifiesList.sort();
		usesList.sort();
		modifiesList.unique();
		usesList.unique();

		//for testing purposes
		/*cout << "modifies: ";
		for (list<string>::iterator listIter = modifiesList.begin(); listIter != modifiesList.end(); ++listIter) {
			cout << *listIter;
			cout << " ,";
		}
		cout << endl;
		cout << "uses: ";
		for (list<string>::iterator listIter = usesList.begin(); listIter != usesList.end(); ++listIter) {
			cout << *listIter;
			cout << " ,";
		}
		cout << endl;*/
	}
}

int Parser::countNumOfLeftBraces(string str) {
	return std::count(str.begin(), str.end(), '{');
}

int Parser::countNumOfRightBraces(string str) {
	return std::count(str.begin(), str.end(), '}');
}

void Parser::processWhile(list<string>::iterator it, list<string>& stmtList, list<int>& modifiesList, list<int>& usesList) {
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
			braces.pop();
		}
		switch (getTypeOfStatement(*it)) {
		case 0: processAssignment(*it, tempModifiesList, tempUsesList); break;
		case 1: break;
		case 2: break;
		case 3: processWhile(it, stmtList, tempModifiesList, tempUsesList); break;
		case 4: break;//for if
		case 5: break;//for else
		case 6: break;//for invalid statement
		}

		modifiesList.splice(modifiesList.end(), tempModifiesList);
		usesList.splice(usesList.end(), tempUsesList);

		++it;
	}
}

void Parser::processAssignment(string str, list<int>& modifiesList, list<int>& usesList) {
	string::iterator it;
	string variable = "";
	modifiesList.clear();
	usesList.clear();

	for (it = str.begin(); it != str.end(); ++it) {
		if (isMathSymbol(*it) || isSemicolon(*it)) {
			if (isVariable(variable)) {
				VarTable varTable = pkb.getVarTable();
				int varID;
				if (varTable.get_ID(variable) == -1) {
					varID = varTable.insert_Var(variable);
				}
				else {
					varID = varTable.get_ID(variable);
				}
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

int Parser::getTypeOfStatement(string str) {
	regex assignment("(([[:alpha:]])([[:alnum:]]+)*)=(.*);\\}*");
	regex procDeclaration("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{");
	regex procCall("call(([[:alpha:]])([[:alnum:]]+)*);\\}*");
	regex whileStmt("while(([[:alpha:]])([[:alnum:]]+)*)\\{");
	regex ifStmt("if(([[:alpha:]])([[:alnum:]]+)*)then\\{");
	regex elseStmt("else\\{");

	if (regex_match(str, assignment)) {
		return 0;
	}
	else if (regex_match(str, procDeclaration)) {
		return 1;
	}
	else if (regex_match(str, procCall)) {
		return 2;
	}
	else if (regex_match(str, whileStmt)) {
		return 3;
	}
	else if (regex_match(str, ifStmt)) {
		return 4;
	}
	else if (regex_match(str, elseStmt)) {
		return 5;
	}
	else {
		return 6;
	}
}