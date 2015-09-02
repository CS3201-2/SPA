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
	stack<ASTNode*> _nodeInProcess;

public:
	AST(string);
	void updateAST(string);

private:
	NodeType getLineType(string line);
	void createProcNode(ASTNode*);
	void createAssignNode(ASTNode*, string);
	void createCallNode(ASTNode*);
	void createWhileNode(ASTNode*);
	void createIfNode(ASTNode*);
	ASTNode createAssignTree(string);
};

#endif
