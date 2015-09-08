#include"StackNode.h"

StackNode::StackNode(ASTNode* treeNode)
{
	this->treeNode = treeNode;
	next = NULL;
}

