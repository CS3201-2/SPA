#include<string>
#include<regex>
#include<unordered_map>
#include<stack>
#include"ASTNode.h"

using namespace std;

class ExpressionTree
{
	ASTNode _root;
	unordered_map<char, int> _inStack;
	unordered_map<char, int> _outStack;
	stack<char> operatorStack;
	string _operandBuffer;
	string _output;

public:
	ExpressionTree(string);
	ASTNode getRoot();
private:
	void constructTree(string);
	string getSuffix(string);
	void buildTree(string);
	void initializeStack();
	bool isOperand(char);
	void updateOperandBuffer(char);
	void extractOperandBuffer();
	void solveOperator(char);
};

