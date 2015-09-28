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
	Parser parser;

	list<pair<int, string>> sourceList = parser.prepareSourceList(source);

	if (!syntaxCheck(sourceList)) {
		cout << endl << endl;
		cout << "syntax wrong!!!" << endl << endl << endl;
	}
	else {
		ctrPKB = parse(sourceList);
	}


	//for testing
	/*cout << "parsed source list" << endl;
	for (list<pair<int, string>>::iterator it = sourceList.begin(); it != sourceList.end(); ++it) {
		//cout << (*it).first << " : ";
		cout << (*it).second << endl;
	}*/
	//cout << "end of printing" << endl;
	//for testing ends



	/*if (!syntaxCheck()) {
		cerr << "The SOURCE file got sytax error!" << endl;
		exit(1);
	} else {
		ctrPKB = parse();
	}*/

}

bool Controller::syntaxCheck(list<pair<int, string>>& sourceList) {
	Checker checker;
	return checker.isSyntaxCorrect(sourceList);
}

void Controller::testingPKB() {
	cout << endl;
	cout << "testing" << endl;
	cout << endl;

	

	//change below for testing purpose
	ctrPKB.getProcTable().printMap();
	ctrPKB.getVarTable().printMap();
	ctrPKB.getModifies().sortMap();
	ctrPKB.getModifies().printMap();
	ctrPKB.getUses().sortMap();
	ctrPKB.getUses().printMap();
	ctrPKB.getCalls().printCallsMap();
	/*ctrPKB.getFollows().printFollowsMap();
	ctrPKB.getParent().sortParentMap();
	ctrPKB.getParent().printParentMap();*/

	cout << "end of testing" << endl;
}

PKB Controller::parse(list<pair<int, string>> sourceList) {
	Parser parser;
	return parser.parseSource(sourceList);
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