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

	//Note: must change back, now it is skipping checker!!!
	if (!syntaxCheck(sourceList)) {
		cout << endl << endl;
		cout << "syntax wrong!!!" << endl << endl << endl;
	}
	else {
		parser.parseSource(sourceList);
	}


	//for testing
	/*cout << "parsed source list" << endl;
	for (list<pair<int, string>>::iterator it = sourceList.begin(); it != sourceList.end(); ++it) {
		//cout << (*it).first << " : ";
		cout << (*it).second << endl;
	}*/
	//cout << "end of printing" << endl;
	//for testing ends
}

bool Controller::syntaxCheck(list<pair<int, string>>& sourceList) {
	Checker checker;
	return checker.isSyntaxCorrect(sourceList);
}

void Controller::logPKB() {
	cout << endl;
	cout << "logging" << endl;
	cout << endl;

	PKB::getPKBInstance()->logVarTable();
	PKB::getPKBInstance()->logProcTable();
	PKB::getPKBInstance()->logModifies();
	PKB::getPKBInstance()->logUses();
	PKB::getPKBInstance()->logFollows();
	PKB::getPKBInstance()->logFollowsStar();
	PKB::getPKBInstance()->logParent();
	PKB::getPKBInstance()->logParentStar();
	PKB::getPKBInstance()->logPattern();


	//change below for testing purpose
	/*list<int> followsFirst = ctrPKB.getWhileList();
	for (list<int>::iterator it = followsFirst.begin(); it != followsFirst.end(); ++it) {
		cout << *it << ", ";
	}
	cout << endl;*/
	cout << "end of logging" << endl;
}

list<string> Controller::processQuery(string query) {
	QueryController queryController;
	return queryController.processQueries(query);
	return list<string>();
}