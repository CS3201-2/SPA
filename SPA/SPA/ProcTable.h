#include <string>
#include <map>

using namespace std;

#ifndef ProcTable_H
#define ProcTable_H

class ProcTable
{
public:
	ProcTable();

	int getID(string);

	int insertProc(string);

	int getSize();

	void printMap();

private:
	map<string,int> procTable;
	
};

#endif
