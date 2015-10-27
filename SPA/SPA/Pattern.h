#include "SPALog.h"
#include <list>
#include <string>
#include <map>
#include <stack>
#include <algorithm>
#include <iterator>

using namespace std;


#ifndef Pattern_H
#define Pattern_H

class Pattern{

public:
	Pattern();
	void setPattern(int, string, string);
	list<int> getAssignWithFirstExact(string);
	list<int> getAssignWithSecondExact(string);
	list<int> getAssignWithSecond(string);
	list<int> getAssignWithBothExact(string, string);
	list<int> getAssignWithBoth(string, string);
	list<int> getIfWithFirstExact(string);
	list<int> getWhileWithFisrtExact(string);
	void sortAndUnifyMap();
	void logPattern();

private:
	map<int, pair<string, string>> assignMap;
	map<string, list<int>> ifMap;
	map<string, list<int>> whileMap;
	map<string, list<int>> assignLeftMap;
	map<string, list<int>> assignRightMap;

	void insertToMap(map<string, list<int>>&, string, int);
	string convertToPostFix(string);
	int getPriority(string);
	bool isParenthesis(string);
	bool isOperator(string);
	string removeAllSpace(string);
	list<string> parseExpression(string);
	list<string> getVariableList(string);
	string getFirstVar(string);
};

#endif