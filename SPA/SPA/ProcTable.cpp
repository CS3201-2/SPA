#include "ProcTable.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;

// constructor
ProcTable::ProcTable(void)
{
}

int ProcTable::getIndex(string procName) {
	if (procTable.find(procName) != procTable.end()) {
		return procTable.at(procName);
	}
	else {
		return insertProc(procName);
	}
}

int ProcTable::getID(string procName)
{
	if (procTable.find(procName) != procTable.end()) {
		return procTable.at(procName);
	}
	else {
		return 0;
	}
}

int ProcTable::insertProc(string procName)
{
	int id = 0 - (procTable.size() + 1);
	procTable.insert(std::pair<string, int>(procName, id));
	return id;
}

int ProcTable::getSize()
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