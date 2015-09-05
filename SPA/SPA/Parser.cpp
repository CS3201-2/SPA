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
	
	list<std::pair<int, string>> sourceCodeList;

	buildSourceCodeList(source, sourceCodeList);

	processSourceCodeList(sourceCodeList);

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

	Modifies modifies = pkb.getModifies();
	Uses uses = pkb.getUses();

	for (list<std::pair<int,string>>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		switch (getTypeOfStatement(*it)){
		case 0: processAssignment(*it, modifiesList, usesList); break;
		case 1: break;
		case 2: break;
		case 3: processWhile(it, stmtList, modifiesList, usesList); break;
		case 4: break;//for if
		case 5: break;//for else
		case 6: break;//for invalid statement
		default: break;
		}

		/*modifiesList.sort();
		usesList.sort();
		modifiesList.unique();
		usesList.unique();*/

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

void Parser::processAssignment(std::pair<int,string> pair, list<int>& modifiesList, list<int>& usesList) {
	string str = pair.second;
	int stmtNumber = pair.first;
	string::iterator it;
	string variable = "";

	modifiesList.clear();
	usesList.clear();

	// clarification of uses and modification: Uses and Modifies are map{var_id, list<stmt_#>}, 
	// so modifiesList and usesList no use here.

	//and one more thing to consider here: the function pkb.getVarTable() should return a reference object.

	for (it = str.begin(); it != str.end(); ++it) {
		if (isMathSymbol(*it) || isSemicolon(*it)) {
			if (isVariable(variable)) {
				VarTable varTable = pkb.getVarTable();
				int varID = varTable.get_ID(variable);

				//if (flag) {
					//modifies.set_modifies_stmt(varID, stmtNumber);
				//}
				if (modifiesList.empty()) {
					modifiesList.push_back(varID);
				}
				else {
					usesList.push_back(varID);
					//uses.set_uses_stmt(varID, stmtNumber);
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
	regex assignment("(([[:alpha:]])([[:alnum:]]+)*)=(.*);\\}*");
	regex procDeclaration("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{");
	regex procCall("call(([[:alpha:]])([[:alnum:]]+)*);\\}*");
	regex whileStmt("while(([[:alpha:]])([[:alnum:]]+)*)\\{");
	regex ifStmt("if(([[:alpha:]])([[:alnum:]]+)*)then\\{");
	regex elseStmt("else\\{");

	string str = pair.second;

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