#include "PKB.h"
#include <string>
#include <list>
#include <stack>

using namespace std;

#ifndef Parser_H
#define Parser_H

class Parser
{
public:
	Parser();

	PKB parseSource(list<pair<int, string>>);

	void trim(string&);

	void addNewLineString(string&);

	void buildSourceCodeList(string, list<std::pair<int, string>>&);

	void processSourceCodeList(list<pair<int, string>>&);

	list<pair<int, string>> prepareSourceList(string);
private:
	PKB pkb;
	Modifies& modifies = pkb.getModifies();
	Uses& uses = pkb.getUses();
	Follows& follows = pkb.getFollows();
	Parent& parent = pkb.getParent();
	Pattern& pattern = pkb.getPattern();
	void processNestedStmt(list<std::pair<int, string>>::iterator&, list<std::pair<int, string>>&,
		list<int>&, list<int>&, list<int>&, int, list<stack<string>>&, list<pair<int, pair<int, string>>>&);
	int countNumOfLeftBraces(std::pair<int, string>);
	int countNumOfRightBraces(std::pair<int, string>);
	void processAssignment(std::pair<int, string>, list<int>&, list<int>&);
	bool isVariable(string);
	bool isSemicolon(char);
	bool isMathSymbol(char);
	int getTypeOfStatement(string);
	string getProcName(int, string);
	string getProcNameCallStmt(string);
	void processPatternStmt(pair<int, string>, int);
	string getControlVarName(int, string);
	void processCalledProcList(list<pair<int, pair<int, string>>>);
};

#endif
