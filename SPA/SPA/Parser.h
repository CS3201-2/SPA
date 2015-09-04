#include "PKB.h"
#include <string>
#include <list>

using namespace std;

#ifndef Parser_H
#define Parser_H

class Parser
{
public:
	Parser();

	PKB parseSource(string);

	void trim(string& line);

	void addNewLineString(string& content);

	void buildSourceCodeList(string content, list<string>& list);

	void processSourceCodeList(list<string>& list);

private:
	PKB pkb;
	void processWhile(list<string>::iterator it, list<string>& stmtList, list<int>& modifiesList, list<int>& usesList);
	int countNumOfLeftBraces(string str);
	int countNumOfRightBraces(string str);
	void processAssignment(string str, list<int>& modifiesList, list<int>& usesList);
	bool isVariable(string str);
	bool isSemicolon(char ch);
	bool isMathSymbol(char ch);
	int getTypeOfStatement(string str);
};

#endif
