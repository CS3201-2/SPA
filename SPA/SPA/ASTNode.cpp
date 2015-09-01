#include "ASTNode.h"

using namespace std;

//Constructor
ASTNode::ASTNode(NodeType type, string content)
{
	_nodeType = type;
	_nodeContent = content;
	_children.clear();
	_parent = NULL;
}

NodeType ASTNode::getNodeType()
{
	return _nodeType;
}

string ASTNode::getContent()
{
	return _nodeContent;
}

void ASTNode::addChildren(ASTNode* node)
{
	_children.push_back(node);
	(*node).setParent(this);
}

list<ASTNode*> ASTNode::getChildren()
{
	return _children;
}

ASTNode* ASTNode::getParent()
{
	return _parent;
}

void ASTNode::setParent(ASTNode* node)
{
	_parent = node;
}