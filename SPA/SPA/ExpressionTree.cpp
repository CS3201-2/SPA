#include "ExpressionTree.h"

const regex kOperator("[+|-|*|/]");
const regex kNumber("[0-9]+");

ExpressionTree::ExpressionTree(string str)
{
	_suffixConvertor.acceptExpression(str);
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
	string subject = _suffix;
	string* ptr = &subject;
	string temp = getNextElement(ptr);
	while (temp != "")
	{
		solveElement(temp);
		temp = getNextElement(ptr);
	}
	_root = _nodeStack.top();
}

string ExpressionTree::getNextElement(string* str)
{
	if (*str == "the_end")
	{
		return "";
	}
	int index = (*str).find_first_of(',');
	if (index == -1)
	{
		string temp = *str;
		*str = "the_end";
		return temp;
	}
	else
	{
		string temp = (*str).substr(0, index);
		*str = (*str).substr(index + 1, (*str).length() - index);
		return temp;
	}
}

void ExpressionTree::solveElement(string str)
{
	if (isOperator(str))
	{
		ASTNode temp1 = _nodeStack.top();
		_nodeStack.pop();
		ASTNode temp2 = _nodeStack.top();
		_nodeStack.pop();
		ASTNode temp3 = createOperatorNode(str);
		temp3.addChildren(temp1);
		temp3.addChildren(temp2);
		_nodeStack.push(temp3);
	}
	else
	{
		ASTNode temp = createOperandNode(str);
		_nodeStack.push(temp);
	}
}

bool ExpressionTree::isOperator(string str)
{
	return regex_match(str, kOperator);
}

bool ExpressionTree::isNumber(string str)
{
	return regex_match(str, kNumber);
}

ASTNode ExpressionTree::createOperatorNode(string str)
{
	NodeType tempType = default;
	if (str == "+")
	{
		tempType = NodeType::plus;
	}
	else if (str == "-")
	{
		tempType = NodeType::minus;
	}
	else if (str == "*")
	{
		tempType = NodeType::times;
	}
	else
	{
		tempType = NodeType::divide;
	}
	ASTNode temp(tempType);
	return temp;
}

ASTNode ExpressionTree::createOperandNode(string str)
{
	if (isNumber(str))
	{
		ASTNode temp(constant, str);
		return temp;
	}
	else
	{
		ASTNode temp(variable, str);
		return temp;
	}
}
