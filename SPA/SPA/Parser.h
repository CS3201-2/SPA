#include "PKB.h"
#include <string>
#include <list>
#include <stack>
#include <regex>


using namespace std;

#ifndef Parser_H
#define Parser_H

class Parser
{
public:
	Parser();

	list<pair<int, string>> prepareSourceList(string);
	void parseSource(list<pair<int, string>>);
	

private:
	void trim(string&);
	void addNewLineString(string&);
	void buildSourceCodeList(string, list<std::pair<int, string>>&);
	void processSourceCodeList(list<pair<int, string>>&);

	void processNestedStmt(list<std::pair<int, string>>::iterator&, list<std::pair<int, string>>&,
		list<int>&, list<int>&, list<int>&, int, list<stack<string>>&, list<pair<int, pair<int, string>>>&);
	int countNumOfLeftBraces(std::pair<int, string>);
	int countNumOfRightBraces(std::pair<int, string>);
	void processAssignment(std::pair<int, string>, list<int>&, list<int>&);
	bool isVariable(string);
	bool isSemicolon(char);
	bool isMathSymbol(char);
	int getTypeOfStmt(string);
	string getProcName(int, string);
	string getProcNameCallStmt(string);
	void processPatternStmt(pair<int, string>, int);
	string getControlVarName(int, string);
};

#endif
