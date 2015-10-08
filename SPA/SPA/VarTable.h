#include <string>
#include <map>
#include "SPALog.h"

using namespace std;


#ifndef VarTable_H
#define VarTable_H

class VarTable{

public:
	VarTable();

	int insertVar(string);
	int getVarID(string); 	//return 0 for invalid varName input
	string getVarName(int);
	size_t getVarTableSize();
	void logVarTable();
	void setVarTableReverse();

private:
	map<string, int> varTable;
	map<int, string> varTableReverse;
};

#endif