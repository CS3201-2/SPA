#include "PKB.h"
#include "DesignExtractor.h"
#include <string>
#include <list>
#include <stack>
#include <iostream>

using namespace std;


#ifndef Checker_H
#define Checker_H

class Checker {

public:
	Checker();

	bool isSyntaxCorrect(list<pair<int, string>>&);

private:
	bool processNestedStmt(list<pair<int, string>>::iterator&, list<pair<int, string>>&,
		stack<int>&, list<pair<int, string>>&, int, list<stack<int>>&);
	bool isAssignmentValid(string);
	bool isCallValid();
	bool isCallsStarValid();
	string getProcName(int, string);
	string getControlVarName(int, string);
	int countNumOfLeftBraces(string);
	int countNumOfRightBraces(string);
	int countNumOfLeftParenthesis(string);
	int countNumOfRightParenthesis(string);
	bool popBrackets(stack<int>&, string);

	int getTypeOfStmt(string);
	bool isOperator(string);
	bool isParenthesis(string);
	bool isValidName(string);
	bool isConstant(string);
	bool isAllClosingBraces(string);
	vector<string> parseExpression(string);
};

#endif