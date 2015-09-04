#include "Parser.h"
#include "PKB.h"
#include <string>
#include <list>
#include <algorithm>

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
	list<string> modifiesList;
	list<string> usesList;

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