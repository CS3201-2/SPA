#include "VarTable.h"


// construactor
VarTable::VarTable(void){
}

int VarTable::insertVar(string varName) {
	if (varTable.find(varName) != varTable.end()) {
		return varTable.at(varName);
	}
	else {
		int id = varTable.size() + 1;
		varTable[varName] = id;
		return id;
	}
}

int VarTable::getVarID(string varName) {
	if (varTable.find(varName) != varTable.end()) {
		return varTable.at(varName);
	}
	else {
		return 0;
	}
}

string VarTable::getVarName(int varID) {
	for (map<string, int>::iterator it = varTable.begin(); it != varTable.end(); ++it) {
		if ((*it).second == varID) {
			return (*it).first;
		}
	}

	//dummy return, should never be used
	return "";
}

size_t VarTable::getVarTableSize() {
	return varTable.size();
}

void VarTable::logVarTable() {
	string str = "variable table\n";
	for (map<string, int>::iterator it = varTable.begin(); it != varTable.end(); ++it) {
		str += (*it).first + ": " + to_string((*it).second) + "\n";
	}
	str += "\n";

	str += "variable table reverse\n";
	for (map<int, string>::iterator it = varTableReverse.begin(); it != varTableReverse.end(); ++it) {
		str += to_string((*it).first) + ": " + (*it).second + "\n";
	}
	str += "\n";

	SPALog::log(str);
}

void VarTable::setVarTableReverse() {
	for (map<string, int>::iterator it = varTable.begin(); it != varTable.end(); ++it) {
		varTableReverse[(*it).second] = (*it).first;
	}
}