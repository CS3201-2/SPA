#include "ExpressionTree.h"

ExpressionTree::ExpressionTree(string str):_suffixConvertor(Suffix(str))
{
	constructTree();
}

ASTNode ExpressionTree::getRoot()
{
	return _root;
}

void ExpressionTree::constructTree()
{
	_suffix = _suffixConvertor.getSuffix();
	buildTree();
}

void ExpressionTree::buildTree()
{

}
