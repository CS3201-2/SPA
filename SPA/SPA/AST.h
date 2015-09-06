#include <string>
#include <list>
#include <stack>
#include <regex>
#include "ASTNode.h"
#include "ExpressionTree.h"
#include "NodeTypeEnum.h"
using namespace std;

#ifndef AST_H
#define AST_H

class AST
{
	ASTNode* _root;
	ASTNode* _nodeBefore;
	stack<ASTNode> _nodeInProcess;
	list<pair<int, string>> _code;

public:
	AST(list<pair<int, string>>);
	void constructTree();
	ASTNode getRoot();

private:
	void updateAST(string);
	NodeType getLineType(string line);
	ASTNode createProgNode(string);
	void createProcNode(ASTNode*, string);
	void createAssignNode(ASTNode*, string);
	void createCallNode(ASTNode*, string);
	void createWhileNode(ASTNode*, string);
	void createIfNode(ASTNode*, string);
	void addExpression(ASTNode*, string);
	bool containsRightCurlyBracket(string);
	bool containsLeftCurlyBracket(string);
	string getVariable(string);
	string getExpression(string);
};

#endif
