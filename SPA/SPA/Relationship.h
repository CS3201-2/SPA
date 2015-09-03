#pragma once
#include <string>
#include <list>

using namespace std;

class QueryValidator {
private:
	int numOfArgs;
	list<string> arg1, arg2;
public:
	void setNumOfArgs(int num);
	void setArg1(list<string> arg);
	void setArg2(list<string> arg);
	bool isNumOfArgsEqual(int num);
	bool isArg1Valid();
	bool isArg2Valid();
};
