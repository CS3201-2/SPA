#include "ProcTable.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;

// constructor
ProcTable::ProcTable(void)
{
}

int ProcTable::insertProc(string procName) {
	if (procTable.find(procName) != procTable.end()) {
		return procTable.at(procName);
	}
	else {
		int id = 0 - (procTable.size() + 1);
		procTable[procName] = id;
		return id;
	}
}

int ProcTable::getProcID(string procName) {
	if (procTable.find(procName) != procTable.end()) {
		return procTable.at(procName);
	}
	else {
		return 0;
	}
}

string ProcTable::getProcName(int procID) {
	for (map<string, int>::iterator it = procTable.begin(); it != procTable.end(); ++it) {
		if ((*it).second == procID) {
			return (*it).first;
		}
	}
}

void ProcTable::printProcTable() {
	cout << "procedure table" << endl;
	for (map<string, int>::iterator it = procTable.begin(); it != procTable.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second << endl;
	}
	cout << endl;
}