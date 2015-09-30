#include <string>
#include <map>

using namespace std;

#ifndef ProcTable_H
#define ProcTable_H

class ProcTable
{
public:
	ProcTable();

	//procTable
	int insertProc(string);
	int getProcID(string); 	//return 0 for invalid procName input
	string getProcName(int);
	void printProcTable();

private:
	map<string,int> procTable;
	
};

#endif
