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
