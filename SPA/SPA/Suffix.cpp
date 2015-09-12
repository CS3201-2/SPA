#include "Suffix.h"

const regex _operand("[0-9]|[a-z]|[A-Z]");
const regex _operator("[+|-|*|/]");
const string kErrorExpressionType = "Error: the type of this charactor is ambiguous!";

Suffix::Suffix()
{
	_operandBuffer = "";
	_suffix = "";
	initializeStack();
}

void Suffix::acceptExpression(string s)
{
	computeSuffix(s);
}

string Suffix::getSuffix()
{
	return _suffix;
}

void Suffix::computeSuffix(string str)
{
	string operandBuffer = "";
	string output = "";

	int i;
	for (i = 0; i < (int)str.length(); i++)
	{
		if (isOperand(str[i]))
		{
			updateOperandBuffer(str[i]);
		}
		else
		{
			extractOperandBuffer();
			string temp;
			stringstream ss;
			ss << str[i];
			ss >> temp;
			if (regex_match(temp, _operator))
			{
				solveOperator(str[i]);
			}
		}

	}
	extractOperandBuffer();
	if (_operatorStack.top() != '#')
	{
		_suffix += _operatorStack.top();
	}
}

void Suffix::initializeStack()
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

bool Suffix::isOperand(char c)
{
	string temp;
	stringstream ss;
	ss << c;
	ss >> temp;
	return regex_match(temp, _operand);
}

void Suffix::updateOperandBuffer(char c)
{
	_operandBuffer += c;
}

void Suffix::extractOperandBuffer()
{
	if (!(_operandBuffer == ""))
	{
		_suffix += _operandBuffer;
		_operandBuffer = "";
	}
}

void Suffix::solveOperator(char c)
{
	switch (_outStack[c]) {

	case 6://c is '('
		_operatorStack.push(c);
		break;
	case 1://c is ')'
		popStack('(');
		break;
	case 2://c is +-
		compareOperator(c);
		break;
	case 4://c is */
		compareOperator(c);
		break;
	default:
		throw kErrorExpressionType;
	}
}

void Suffix::popStack(char c)
{
	char temp = _operatorStack.top();
	while (temp != c)
	{
		_suffix += temp;
		_operatorStack.pop();
		temp = _operatorStack.top();
	}
	_operatorStack.pop();
}

void Suffix::compareOperator(char c)
{
	char temp = _operatorStack.top();
	while (_outStack[c] <= _inStack[temp])
	{
		_suffix += temp;
		_operatorStack.pop();
		temp = _operatorStack.top();
	}
	_operatorStack.push(c);
}
