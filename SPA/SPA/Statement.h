#include<string>
#include"StatementType.h"

using namespace std;
#ifndef Statement_H
#define Statement_H

class Statement
{
	int _stmtNumber;
	string _stmtContent;
	StatementType _stmtType;
public:
	Statement();
	Statement(int, string, StatementType);
	int getNumber();
	string getContent();
	StatementType getType();
	void setNumber(int);
	void setContent(string);
	void setType(StatementType);
	bool isEqual(Statement);
	~Statement();
};
#endif

