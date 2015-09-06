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

	void buildSourceCodeList(string content, list<std::pair<int,string>>& list);

	void processSourceCodeList(list<pair<int, string>>& list);

private:
	PKB pkb;
	void processWhile(list<std::pair<int,string>>::iterator it, list<std::pair<int, string>>& stmtList, list<int>& modifiesList, list<int>& usesList);
	int countNumOfLeftBraces(std::pair<int, string> pair);
	int countNumOfRightBraces(std::pair<int, string> pair);
	void processAssignment(std::pair<int,string> pair, list<int>& modifiesList, list<int>& usesList);
	bool isVariable(string str);
	bool isSemicolon(char ch);
	bool isMathSymbol(char ch);
	int getTypeOfStatement(std::pair<int, string> pair);
};

#endif
