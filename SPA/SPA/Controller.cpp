#include "Controller.h"
#include "Checker.h"
#include "Parser.h"
#include "PKB.h"
#include "QueryController.h"
#include "SPALog.h"
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

	logSourceCode(sourceList);

	if (!syntaxCheck(sourceList)) {
		cout << endl << endl;
		cout << "syntax wrong!!!" << endl << endl << endl;
	}
	else {
		parser.parseSource(sourceList);
		DesignExtractor de = DesignExtractor();
		de.setFollowsStar();
		de.setParentStar();
		de.resetModifies();
		de.resetUses();
	}
	cout << "end of checker and parsing" << endl;


	//for testing
	/*cout << "parsed source list" << endl;
	for (list<pair<int, string>>::iterator it = sourceList.begin(); it != sourceList.end(); ++it) {
		//cout << (*it).first << " : ";
		cout << (*it).second << endl;
	}*/
	//cout << "end of printing" << endl;
	//for testing ends
}

void Controller::logSourceCode(list<pair<int, string>> sourceList) {
	string str;
	for (auto& x : sourceList) {
		if (x.first < 0) {
			str += to_string(x.first) + " : " + x.second + "\n";
		}
		else if (x.first < 10) {
			str += to_string(x.first) + "  : " + x.second + "\n";
		}
		else if (x.first < 100){
			str += to_string(x.first) + " : " + x.second + "\n";
		}
		else {
			str += to_string(x.first) + ": " + x.second + "\n";
		}
	}

	SPALog::log(str);
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
	PKB::getPKBInstance()->logCalls();
	PKB::getPKBInstance()->logCallsStar();


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