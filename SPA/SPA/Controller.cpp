#include "Controller.h"
#include "Checker.h"
#include "Parser.h"
#include "PKB.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

typedef list<string> StringList;
Controller::Controller( string sourceCode) {
	source = sourceCode;
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