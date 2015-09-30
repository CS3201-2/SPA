#include <string>
#include <map>
#include <iostream>

using namespace std;


#ifndef VarTable_H
#define VarTable_H

class VarTable
{
public:
	VarTable();

	int insertVar(string);
	int getVarID(string); 	//return 0 for invalid varName input
	string getVarName(int);
	size_t getVarTableSize();
	void printVarTable();

private:
	map<string, int> varTable;
};

#endif