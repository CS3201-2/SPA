#include "Controller.h"

bool SYNTAX_CORRECT = true;
bool SYNTAX_WRONG = false;

typedef list<string> StringList;
Controller::Controller() {
}

void Controller::intializeCode(string code) {
	source = code;
}

bool Controller::getIsSyntaxCorrect() {
	return _isSyntaxCorrect;
}

void Controller::processSource() {
	Parser parser;

	ifstream inputFile;

	inputFile.open("config/LogLevel.txt");

	if (inputFile.fail()) {
		SPALog::getSPALogInstance()->setLogLevel(0);
	}
	else {
		std::string level((std::istreambuf_iterator<char>(inputFile)),
			(std::istreambuf_iterator<char>()));
		SPALog::getSPALogInstance()->setLogLevel(stoi(level));
	}

	list<Statement> sourceList = parser.prepareSourceList(source);

	logSourceCode(sourceList);

	if (!isSyntaxCorrect(sourceList)) {
		_isSyntaxCorrect = SYNTAX_WRONG;
		SPALog::getSPALogInstance()->logWithLevel(ZERO_LEVEL, "syntax wrong!!!");
	}
	else {
		_isSyntaxCorrect = SYNTAX_CORRECT;
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

	SPALog::getSPALogInstance()->logWithLevel(LOW_LEVEL, str);
}

bool Controller::isSyntaxCorrect(list<Statement>& sourceList) {
	Checker checker;
	return checker.isSyntaxCorrect(sourceList);
}

void Controller::logPKB() {
	PKB::getPKBInstance()->logWhileList();
	PKB::getPKBInstance()->logAssignList();
	PKB::getPKBInstance()->logCallList();
	PKB::getPKBInstance()->logIfList();
	PKB::getPKBInstance()->logConstantList();
	PKB::getPKBInstance()->logParentList();
	PKB::getPKBInstance()->logStmtList();
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
}

list<string> Controller::processQuery(string query) {
	QueryController queryController;
	return queryController.processQueries(query);
}