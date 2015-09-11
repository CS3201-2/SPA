#include "Prefix.h"

const regex kOperand("[0-9]|[a-z]|[A-Z]");
const string kErrorExpressionType = "Error: the type of this charactor is ambiguous!";

Prefix::Prefix()
{
	_operandBuffer = "";
	_operatorStack.push('#');
	_operatorStack.empty();
	_operandQ.empty();
	initializeStack();
}

void Prefix::acceptExpression(string s)
{
	_prefix.clear();
	_operandBuffer = "";
	_operatorStack.push('#');
	computePrefix(s);
}

list<string> Prefix::getPrefix()
{
	return _prefix;
}

void Prefix::print()
{
	for (list<string>::iterator it = _prefix.begin();
	it != _prefix.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void Prefix::computePrefix(string str)
//assert(str is without space)
{
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
			_prefix.push_back(toString(str[i]));
		}

	}
	extractOperandBuffer();
	_operandQ.push("#");
	popOperand();
}

void Prefix::initializeStack()
{
	_inStack.emplace('#', 0);
	_inStack.emplace('(', 1);	_outStack.emplace('(', 6);
	_inStack.emplace('+', 3);	_outStack.emplace('+', 2);
	_inStack.emplace('-', 3);	_outStack.emplace('-', 2);
	_inStack.emplace('*', 5);	_outStack.emplace('*', 4);
	_inStack.emplace('/', 5);	_outStack.emplace('/', 4);
	_inStack.emplace(')', 6);	_outStack.emplace(')', 1);
}

bool Prefix::isOperand(char c)
{
	return regex_match(toString(c), kOperand);
}

void Prefix::updateOperandBuffer(char c)
{
	_operandBuffer += c;
}

void Prefix::extractOperandBuffer()
{
	if (!(_operandBuffer == ""))
	{
		_operandQ.push(_operandBuffer);
		_operandBuffer = "";
	}
}

void Prefix::solveOperator(char c)
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

void Prefix::popStack(char c)
{
	char temp = _operatorStack.top();
	while (temp != c)
	{
		_prefix.push_back(toString(temp));
		_operatorStack.pop();
		temp = _operatorStack.top();
	}
	_operatorStack.pop();
}

void Prefix::compareOperator(char c)
{
	char temp = _operatorStack.top();
	while (_outStack[c] <= _inStack[temp])
	{
		_prefix.push_back(toString(temp));
		_operatorStack.pop();
		temp = _operatorStack.top();
	}
	_operatorStack.push(c);
}

string Prefix::toString(char c)
{
	string temp;
	stringstream ss;
	ss << c;
	ss >> temp;
	return temp;
}

void Prefix::pushLeft()
{
	string temp = toString(_operatorStack.top());
	while (temp != "#")
	{
		_prefix.push_back(temp);
		_operatorStack.pop();
		temp = toString(_operatorStack.top());
	}
}

void Prefix::popOperand()
{
	string temp = _operandQ.front();
	while (temp != "#")
	{
		_prefix.push_back(temp);
		_operandQ.pop();
		temp = _operandQ.front();
	}
	_operandQ.pop();
}