#include "ASTNode.h"

using namespace std;

//Constructor
ASTNode::ASTNode(NodeType type, string content)
{
	_nodeType = type;
	_nodeContent = content;
	_children = 
}

NodeType ASTNode::getNodeType()
{
	return _nodeType;
}

string ASTNode::getContent()
{
	return _nodeContent;
}

void ASTNode::addChildren(ASTNode node)
{

}

void ASTNode::setParent(ASTNode)
{

}

list<ASTNode> ASTNode::getChildren()
{

}

ASTNode ASTNode::getParent()
{

}

