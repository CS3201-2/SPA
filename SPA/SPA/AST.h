#include <string>
#include <list>
#include "ASTNode.h"
using namespace std;

#ifndef AST_H
#define AST_H

class AST
{
	ASTNode _root;

public:
	AST();
	void updateAST(string);
};

#endif
