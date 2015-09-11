#include<string>
#include <sstream>
#include<iostream>
#include<regex>
#include<unordered_map>
#include<stack>
#include<queue>

using namespace std;

#ifndef Prefix_H
#define Prefix_H

class Prefix
{
	unordered_map<char, int> _inStack;
	unordered_map<char, int> _outStack;
	stack<char> _operatorStack;
	queue<string> _operandQ;
	string _operandBuffer;
	list<string> _prefix;
public:
	Prefix();
	void acceptExpression(string);
	list<string> getPrefix();
	void print();
private:
	void computePrefix(string);
	void initializeStack();
	bool isOperand(char);
	void updateOperandBuffer(char);
	void extractOperandBuffer();
	void solveOperator(char);
	void popStack(char);
	void compareOperator(char);
	string toString(char);
	void pushLeft();
	void popOperand();
};

#endif