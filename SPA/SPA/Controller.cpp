#include "Controller.h"


typedef list<string> StringList;
Controller::Controller() {
}

void Controller::intializeCode(string code) {
	source = code;
}

void Controller::processSource() {
	Parser parser;

	list<Statement> sourceList = parser.prepareSourceList(source);

	logSourceCode(sourceList);

	if (!syntaxCheck(sourceList)) {
		cout << endl << endl;
		cout << "syntax wrong!!!" << endl << endl << endl;
		SPALog::log("syntax wrong!!!");
	}
	else {
		parser.parseSource(sourceList);
		DesignExtractor de = DesignExtractor();
		de.setFollowsStar();
		de.setParentStar();
		de.resetModifies();
		de.resetUses();
		de.setReverseMap();
		parser.buildCFG(sourceList);
		
		logPKB();
	}
	cout << "end of checker and parsing" << endl;
}

void Controller::logSourceCode(list<Statement> sourceList) {
	string str;
	for (auto& x : sourceList) {
		if (x.getNumber() < 0) {
			str += to_string(x.getNumber()) + " : " + x.getContent() + "\n";
		}
		else if (x.getNumber() < 10) {
			str += to_string(x.getNumber()) + "  : " + x.getContent() + "\n";
		}
		else if (x.getNumber() < 100){
			str += to_string(x.getNumber()) + " : " + x.getContent() + "\n";
		}
		else {
			str += to_string(x.getNumber()) + ": " + x.getContent() + "\n";
		}
	}

	SPALog::log(str);
}

bool Controller::syntaxCheck(list<Statement>& sourceList) {
	Checker checker;
	return checker.isSyntaxCorrect(sourceList);
}

void Controller::logPKB() {
	cout << endl;
	cout << "logging" << endl;
	cout << endl;

	PKB::getPKBInstance()->logWhileList();
	PKB::getPKBInstance()->logAssignList();
	PKB::getPKBInstance()->logCallList();
	PKB::getPKBInstance()->logIfList();
	PKB::getPKBInstance()->logConstantList();
	PKB::getPKBInstance()->logParentList();
	PKB::getPKBInstance()->logStmtList();
	PKB::getPKBInstance()->logStmtLstList();
	PKB::getPKBInstance()->logCallStmtProcMap();
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
	PKB::getPKBInstance()->logNext();


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