#include <string>
#include <list>
#include <stack>
#include "ASTNode.h"
#include "NodeTypeEnum.h"
using namespace std;

#ifndef AST_H
#define AST_H

class AST
{
	ASTNode* _root;
	ASTNode* _nodeBefore;
	stack<ASTNode> _nodeInProcess;

public:
	AST(string);
	void updateAST(string);
	ASTNode getRoot();

private:
	NodeType getLineType(string line);
	void createProcNode(ASTNode*, string);
	void createAssignNode(ASTNode*, string);
	void createCallNode(ASTNode*);
	void createWhileNode(ASTNode*);
	void createIfNode(ASTNode*);
	void addExpression(ASTNode*, string);
};

#endif
