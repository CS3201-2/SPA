#include "Statement.h"



Statement::Statement()
{
}

Statement::Statement(int i, string c, StatementType t)
{
	_stmtNumber = i;
	_stmtContent = c;
	_stmtType = t;
}

int Statement::getNumber()
{
	return _stmtNumber;
}

string Statement::getContent()
{
	return _stmtContent;
}

StatementType Statement::getType()
{
	return _stmtType;
}


Statement::~Statement()
{
}
