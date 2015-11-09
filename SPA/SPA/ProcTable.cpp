#include "ProcTable.h"


// constructor
ProcTable::ProcTable(void){
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

	//dummy return, should never be used
	return "";
}

size_t ProcTable::getProcTableSize() {
	return procTable.size();
}

void ProcTable::logProcTable() {
	string str = "procedure table\n";
	for (map<string, int>::iterator it = procTable.begin(); it != procTable.end(); ++it) {
		str += (*it).first + ": " + to_string((*it).second) + "\n";
	}
	str += "\n";

	str += "procedure table reverse\n";
	for (map<int, string>::iterator it = procTableReverse.begin(); it != procTableReverse.end(); ++it) {
		str += to_string((*it).first) + ": " + (*it).second + "\n";
	}
	str += "\n";

	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void ProcTable::setProcTableReverse() {
	for (map<string, int>::iterator it = procTable.begin(); it != procTable.end(); ++it) {
		procTableReverse[(*it).second] = (*it).first;
	}
}