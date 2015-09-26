#include <stack>
#include <string>
#include <iostream>
#include <stack>
#include <list>
#include <map>

using namespace std;

#ifndef Pattern_H
#define Pattern_H

class Pattern
{
public:
	Pattern();
	void setPattern(int, string, string);
	void printMap();
	//getStmtFromLHS, getStmtFromRHS, getStmtFromBoth, getPatternFromStmtLine

private:
	map<int, pair<string, string>> patternMap;
	string convertToPostFix(string);
	int getPriority(string);
	bool isParenthesis(string);
	bool isOperator(string);
};

#endif