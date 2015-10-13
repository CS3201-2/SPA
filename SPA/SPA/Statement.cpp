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

void Statement::setNumber(int stmtNumber) {
	_stmtNumber = stmtNumber;
}

void Statement::setContent(string stmtContent) {
	_stmtContent = stmtContent;
}

void Statement::setType(StatementType stmtType) {
	_stmtType = stmtType;
}

bool Statement::isEqual(Statement statement) {
	if (_stmtNumber != statement.getNumber()) {
		return false;
	}
	if (_stmtContent != statement.getContent()) {
		return false;
	}
	if (_stmtType != statement.getType()) {
		return false;
	}
	return true;
}


Statement::~Statement()
{
}
