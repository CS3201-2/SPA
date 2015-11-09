#include "PKB.h"
#include "Statement.h"
#include "StatementType.h"
#include <string>
#include <list>
#include <stack>

using namespace std;


#ifndef Parser_H
#define Parser_H

class Parser{

public:
	Parser();

	list<Statement> prepareSourceList(string);
	void parseSource(list<Statement>);
	void buildCFG(list<Statement>);

private:
	string removeComment(string);
	void trim(string&);
	void addNewLineBeforeKeywords(string&, string);
	void addNewLineString(string&);
	void buildSourceCodeList(string, list<Statement>&);
	void processSourceCodeList(list<Statement>);
	void processNestedStmt(list<Statement>::iterator&, list<Statement>,
		list<int>&, list<int>&, list<int>&, int, list<stack<string>>&);
	int countNumOfLeftBraces(string);
	int countNumOfRightBraces(string);
	void processAssignment(string str, list<int>&, list<int>&);
	void processCallStmt(int, StatementType, string, list<int>&, list<int>&);
	StatementType getTypeOfStmt(string);
	void processPatternStmt(int, string, StatementType);
	string getControlVarName(StatementType, string);
	string getProcName(StatementType, string);
	
	list<string> parseExpression(string);
	bool isValidName(string);
	bool isConstant(string);
	bool isAllClosingBraces(string);
};

#endif
