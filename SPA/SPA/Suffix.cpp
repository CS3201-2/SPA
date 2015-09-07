#include "Suffix.h"

const regex kOperand("[0-9]|[a-z]|[A-Z]");
const string kErrorExpressionType = "Error: the type of this charactor is ambiguous!";

Suffix::Suffix()
{
	_operandBuffer = "";
	_operatorStack.push('#');
	_operatorStack.empty();
	_operandQ.empty();
	initializeStack();
}

void Suffix::acceptExpression(string s)
{
	_suffix.clear();
	_operandBuffer = "";
	_operatorStack.push('#');
	computeSuffix(s);
}

list<string> Suffix::getSuffix()
{
	return _suffix;
}

void Suffix::print()
{
	for (list<string>::iterator it = _suffix.begin();
	it != _suffix.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void Suffix::computeSuffix(string str)
//assert(str is without space)
{
	int i;
	for (i = 0; i < (int)str.length(); i++)
	{
		if (isOperand(str[i]))
		{
			//cout << "operand found" << endl;
			updateOperandBuffer(str[i]);
		}
		else
		{
			extractOperandBuffer();
			_suffix.push_back(toString(str[i]));
			//solveOperator(str[i]);
		}

	}
	extractOperandBuffer();
	//cout << "queue size is " << _operandQ.size() << endl;
	_operandQ.push("#");
	popOperand();
	//pushLeft();
}

void Suffix::initializeStack()
{
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
	return regex_match(toString(c), kOperand);
}

void Suffix::updateOperandBuffer(char c)
{
	_operandBuffer += c;
}

void Suffix::extractOperandBuffer()
{
	if (!(_operandBuffer == ""))
	{
		//cout << "I push into OperandQ " << _operandBuffer << endl;
		_operandQ.push(_operandBuffer);
		//_suffix.push_back(_operandBuffer);
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
		_suffix.push_back(toString(temp));
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
		_suffix.push_back(toString(temp));
		_operatorStack.pop();
		temp = _operatorStack.top();
	}
	_operatorStack.push(c);
}

string Suffix::toString(char c)
{
	string temp;
	stringstream ss;
	ss << c;
	ss >> temp;
	return temp;
}

void Suffix::pushLeft()
{
	string temp = toString(_operatorStack.top());
	while (temp != "#")
	{
		_suffix.push_back(temp);
		_operatorStack.pop();
		temp = toString(_operatorStack.top());
	}
}

void Suffix::popOperand()
{
	string temp = _operandQ.front();
	//cout << "var is " << temp << endl;
	while (temp != "#")
	{
		_suffix.push_back(temp);
		_operandQ.pop();
		temp = _operandQ.front();
	}
	_operandQ.pop();
}