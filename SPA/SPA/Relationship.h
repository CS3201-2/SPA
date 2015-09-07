#pragma once
#include <string>
#include <vector>

using namespace std;

class Relationship {
private:
	int numOfArgs;
	vector<string> arg1, arg2;
public:
	Relationship();
	Relationship(int num, vector<string> arg1, vector<string> arg2);
	/*void setNumOfArgs(int num);
	void setArg1(vector<string> arg);
	void setArg2(vector<string> arg);*/
	int getNumOfArgs();
	vector<string> getArg1();
	vector<string> getArg2();
	/*bool isNumOfArgsEqual(int num);
	bool isArg1Valid();
	bool isArg2Valid();*/
};
