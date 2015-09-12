#include "Controller.h"
#include "Checker.h"
#include "Parser.h"
#include "PKB.h"
#include "QueryController.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

typedef list<string> StringList;
Controller::Controller() {
}

void Controller::intializeCode(string code) {
	source = code;
}

void Controller::processSource() {
	if (!syntaxCheck()) {
		cerr << "The SOURCE file got sytax error!" << endl;
		exit(1);
	} else {
		ctrPKB = parse();
	}

}

int Controller::syntaxCheck() {
	Checker checker;
	return checker.checkSyntax(source);
}

PKB Controller::parse() {
	Parser parser;
	return parser.parseSource(source);
}

list<string> Controller::processQuery(string query) {
	//string::iterator itr;
	//list<string> processedQueryList;
	
	/*
	while (!query.empty()) {
		size_t index = query.find("\n");
		string temp = query.substr(0, index);
		query = query.substr(index + 1, query.size());
		processedQueryList.push_back(temp);
	}
	*/

	QueryController queryController;
	return queryController.processQueries(query, ctrPKB);
}