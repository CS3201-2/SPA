#include "DesignExtractor.h"
#include "Checker.h"
#include "Parser.h"
#include "PKB.h"
#include "QueryController.h"
#include "SPALog.h"
#include "Statement.h"
#include <string>
#include <list>

using namespace std;


#ifndef Controller_H
#define Controller_H

class Controller{

public:
	Controller();
	void intializeCode(string);
	void processSource();
	list<string> processQuery(string);
	bool getIsSyntaxCorrect();

private:
	string source;
	bool _isSyntaxCorrect;

	void logPKB();
	void logSourceCode(list<Statement>);
	bool isSyntaxCorrect(list<Statement>&);
};

#endif