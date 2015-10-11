#include "PKB.h"
#include <string>
#include <list>
#include <stack>

using namespace std;


#ifndef Parser_H
#define Parser_H

class Parser{

public:
	Parser();

	list<pair<int, string>> prepareSourceList(string);
	void parseSource(list<pair<int, string>>);
	void buildCFG(list<pair<int, string>>);

private:
	void trim(string&);
	void addNewLineBeforeKeywords(string&, string);
	void addNewLineString(string&);
	void buildSourceCodeList(string, list<pair<int, string>>&);
	void processSourceCodeList(list<pair<int, string>>&);

	void processNestedStmt(list<pair<int, string>>::iterator&, list<pair<int, string>>&,
		list<int>&, list<int>&, list<int>&, int, list<stack<string>>&);
	int countNumOfLeftBraces(string);
	int countNumOfRightBraces(string);
	void processAssignment(string str, list<int>&, list<int>&);
	void processCallStmt(int, int, string, list<int>&, list<int>&);
	int getTypeOfStmt(string);
	void processPatternStmt(int, string, int);
	string getControlVarName(int, string);
	string getProcName(int, string);
	
	list<string> parseExpression(string);
	bool isValidName(string);
	bool isConstant(string);
	bool isAllClosingBraces(string);
};

#endif
