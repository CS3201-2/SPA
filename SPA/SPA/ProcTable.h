#include "SPALog.h"
#include <string>
#include <map>
#include <iostream>

using namespace std;


#ifndef ProcTable_H
#define ProcTable_H

class ProcTable
{
public:
	ProcTable();

	int insertProc(string);
	int getProcID(string); 
	string getProcName(int);
	size_t getProcTableSize();
	void logProcTable();

private:
	map<string, int> procTable;
};

#endif