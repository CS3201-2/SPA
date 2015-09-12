#include"AnotherExpressionTree.h"

const regex kOperator("[+|-|*|/]");
const regex kNumber("[0-9]+");

/** constructor **/
AnotherExpressionTree::AnotherExpressionTree()
{
	top = NULL;
}
/** function to clear tree **/
void AnotherExpressionTree::clear()
{
	top = NULL;
}
/** function to push a node **/
void AnotherExpressionTree::push(ASTNode *ptr)
{
	if (top == NULL)
		top = new StackNode(ptr);
	else
	{
		StackNode *nptr = new StackNode(ptr);
		nptr->next = top;
		top = nptr;
	}
}
/** function to pop a node **/
ASTNode* AnotherExpressionTree::pop()
{
	if (top == NULL)
	{
		throw "Underflow";
	}
	else
	{
		ASTNode *ptr = top->treeNode;
		top = top->next;
		return ptr;
	}
}
string AnotherExpressionTree::interpreteExpression(ASTNode * node)
{
	if (isOperator(node))
	{
		ASTNode* left = node->getLeftChild();
		ASTNode* right = node->getRightChild();
		return interpreteExpression(left) + interpreteExpression(right) + 
			node->getNodeType();
	}
	else
	{
		return node->getContent();
	}
}
/** function to get top node **/
ASTNode* AnotherExpressionTree::peek()
{
	return top->treeNode;
}
/** function to insert character **/
void AnotherExpressionTree::insert(string val)
{
	if (isOperator(val))
	{
		ASTNode *nptr = new ASTNode(val);
		nptr->setLeftChild(pop());
		nptr->setRightChild(pop());
		push(nptr);
	}
	else if (isNumber(val))
	{
		ASTNode *nptr = new ASTNode(val, "constant");
		push(nptr);
	}
	else
	{
		ASTNode *nptr = new ASTNode(val, "variable");
		push(nptr);
	}
}

void AnotherExpressionTree::build(string input)
{
	Prefix pf;
	pf.acceptExpression(input);
	list<string> lst = pf.getPrefix();
	lst.reverse();
	for (list<string>::iterator it = lst.begin();
	it != lst.end(); it++)
	{
		insert(*it);
	}
}

bool AnotherExpressionTree::compareExpression(ASTNode* node, string exp)
{
	Suffix sf;
	sf.acceptExpression(exp);
	string suffixExpression = sf.getSuffix();
	string interpretedExpression = interpreteExpression(node);
	return interpretedExpression == suffixExpression;
}

bool AnotherExpressionTree::isOperator(string str)
{
	return regex_match(str, kOperator);
}

bool AnotherExpressionTree::isOperator(ASTNode* node)
{
	string type = node->getNodeType();
	return !(type == "constant" || type== "variable");
}

bool AnotherExpressionTree::isNumber(string str)
{
	return regex_match(str, kNumber);
}
