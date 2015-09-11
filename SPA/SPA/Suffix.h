#include<string>
#include <sstream>
#include<regex>
#include<unordered_map>
#include<stack>

using namespace std;
#ifndef Suffix_H
#define Suffix_H
class Suffix
{
	unordered_map<char, int> _inStack;
	unordered_map<char, int> _outStack;
	stack<char> _operatorStack;
	string _operandBuffer;
	string _suffix;
public:
	Suffix();
	void acceptExpression(string);
	string getSuffix();
private:
	void computeSuffix(string);
	void initializeStack();
	bool isOperand(char);
	void updateOperandBuffer(char);
	void extractOperandBuffer();
	void solveOperator(char);
	void popStack(char);
	void compareOperator(char);	
};
#endif
