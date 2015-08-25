#include <string>
#include <map>

using namespace std;

#ifndef ProcTable_H
#define ProcTable_H

class ProcTable
{
public:
	ProcTable();

	int get_ID(string);

	int insert_Proc(string);

	int get_size();

private:
	map<string,int> procTable;
	
};

#endif
