#include<string>
#include<regex>
#include<unordered_map>
#include<stack>
#include"ASTNode.h"
#include"Suffix.h"
#include"NodeTypeEnum.h"

using namespace std;

class ExpressionTree
{
	ASTNode _root;
	Suffix _suffixConvertor;
	string _suffix;
	stack<ASTNode> _nodeStack;

public:
	ExpressionTree(string);
	ASTNode getRoot();
private:
	void constructTree();
	void buildTree();
	void solveElement(string);
	bool isOperator(string);
	bool isNumber(string);
	string getNextElement(string*);
	ASTNode createOperatorNode(string);
	ASTNode createOperandNode(string);
};