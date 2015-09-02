#include "ExpressionTree.h"

ExpressionTree::ExpressionTree(string str)
{
	constructTree(str);
}

ASTNode ExpressionTree::getRoot()
{
	return _root;
}

void ExpressionTree::constructTree(string str)
{
	string suffix = getSuffix(str);
	buildTree(suffix);
}

string ExpressionTree::getSuffix(string str) 
{
	return;
}

void ExpressionTree::buildTree(string str)
{

}
