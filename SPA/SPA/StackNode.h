#include"ASTNode.h"
using namespace std;

#ifndef StackNode_H
#define StackNode_H
class StackNode
{

public:

	ASTNode *treeNode;

	StackNode *next;

	/** constructor **/
	StackNode(ASTNode*);
};
#endif