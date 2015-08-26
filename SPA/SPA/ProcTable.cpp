#include "ProcTable.h"
#include <map>
#include <string>

using namespace std;

// constructor
ProcTable::ProcTable(void)
{
}

int ProcTable::get_ID(string procName)
{
	return procTable.at(procName);
}

int ProcTable::insert_Proc(string procName)
{
	int id = procTable.size() + 1;
	procTable.insert( std::pair<string,int>( procName, id ) );
	return id;
}

int ProcTable::get_size()
{
	return procTable.size();
}