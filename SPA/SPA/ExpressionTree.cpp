#include "ExpressionTree.h"

const regex _operand("[0-9]|[a-z]|[A-Z]");
const string kErrorExpressionType = "Error: the type of this charactor is ambiguous!";

ExpressionTree::ExpressionTree(string str)
{
	_operandBuffer = "";
	_output = "";
	initializeStack();
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
//assert(str is without space)
{
	string operandBuffer = "";
	string output = "";

	int i;
	for (i = 0; i < str.length; i++)
	{
		if (isOperand(str[i]))
		{
			updateOperandBuffer(str[i]);
			i++;
		} 
		else
		{
			extractOperandBuffer();
			solveOperator(str[i]);
		}
		
	}
	return;
}

void ExpressionTree::buildTree(string str)
{

}

void ExpressionTree::initializeStack()
{
	_operatorStack.push('#');
	_inStack.emplace('#', 0);
	_inStack.emplace('(', 1);	_outStack.emplace('(', 6);
	_inStack.emplace('+', 3);	_outStack.emplace('+', 2);
	_inStack.emplace('-', 3);	_outStack.emplace('-', 2);
	_inStack.emplace('*', 5);	_outStack.emplace('*', 4);
	_inStack.emplace('/', 5);	_outStack.emplace('/', 4);
	_inStack.emplace(')', 6);	_outStack.emplace(')', 1);
}

bool ExpressionTree::isOperand(char c)
{
	return regex_match("" + c, _operand);
}

void ExpressionTree::updateOperandBuffer(char c)
{
	_operandBuffer += c;
}

void ExpressionTree::extractOperandBuffer()
{
	_output += _operandBuffer;
	_output += ',';
	_operandBuffer = "";
}

void ExpressionTree::solveOperator(char c)
{
	switch (_outStack[c]) {
	case 6://c is '('
		_operatorStack.push(c);
		break;
	case 1://c is ')'
		popStack('(');
		break;
	case 2:
		compareOperator(c);
		break;
	case 4:
		compareOperator(c);
		break;
	default:
		throw kErrorExpressionType;
	}
}

void ExpressionTree::popStack(char c)
{
	char temp = _operatorStack.top();
	while (temp != c) 
	{
		_output += temp;
		_output += ',';
		_operatorStack.pop();
		char temp = _operatorStack.top();
	}
}

void ExpressionTree::compareOperator(char c)
{
	char temp = _operatorStack.top();
	while (_outStack[c] <= _inStack[temp])
	{
		_output += temp;
		_output += ',';
		_operatorStack.pop();
	}
	_operatorStack.push(c);
}
