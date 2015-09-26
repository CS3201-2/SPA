#include <string>
#include <list>
#include <regex>
#include <stack>
#include <iostream>
using namespace std;

#ifndef Checker_H
#define Checker_H

class Checker
{
public:
	Checker();

	bool isSyntaxCorrect(list<pair<int, string>>&);

private:
	bool processNestedStmt(list<pair<int, string>>::iterator&, list<pair<int, string>>&, 
		stack<int>&, list<string>&, string, list<stack<int>>&);
	int getTypeOfStmt(string);
	bool isAssignmentValid(string);
	bool isVariable(string);
	bool isConstant(string);
	bool isSemicolon(char);
	bool isOperator(char);
	bool isParenthesis(char);
	string getProcName(string);
	int countNumOfLeftBraces(string);
	int countNumOfRightBraces(string);
	bool popBrackets(stack<int>&, string);
};

#endif