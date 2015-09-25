#include "ProcTable.h"
#include <map>
#include <string>
#include <iostream>

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
	int id = 0 - (procTable.size() + 1);
	procTable.insert(std::pair<string, int>(procName, id));
	return id;
}

int ProcTable::get_size()
{
	return procTable.size();
}

void ProcTable::printMap() {
	for (map<string, int>::iterator it = procTable.begin(); it != procTable.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second << endl;
	}
}