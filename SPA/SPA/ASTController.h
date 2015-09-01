#include <string>
#include <list>
#include "AST.h"
#include "ASTNode.h"

using namespace std;

#ifndef ASTController_H
#define ASTController_H

class ASTController
{
	AST _ASTTree;

public:
	ASTController();
	void updateAST(string);
	list<ASTNode> getChildren(ASTNode);
	ASTNode getParent(ASTNode);
	list<ASTNode> getSiblings(ASTNode);



};

#endif
