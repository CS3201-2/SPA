#include "VarTable.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;

// construactor
VarTable::VarTable(void)
{
}

int VarTable::getIndex(string varName) {
	if (varTable.find(varName) != varTable.end()) {
		return varTable.at(varName);
	}
	else {
		return insertVar(varName);
	}
}

int VarTable::getID(string varName)
{
	if (varTable.find(varName) != varTable.end()) {
		return varTable.at(varName); 
	}
	else {
		return 0;
	}
}

int VarTable::insertVar(string varName)
{
	int id = varTable.size() + 1;
	varTable.insert( std::pair<string,int>( varName, id ) );
	return id;
}

int VarTable::getSize()
{
	return varTable.size();
}

string VarTable::getVarName(int varId) {
	for (map<string, int>::iterator it = varTable.begin(); it != varTable.end(); ++it) {
		if ((*it).second == varId) {
			return (*it).first;
		}
	}
}

void VarTable::printMap() {
	for (map<string, int>::iterator it = varTable.begin(); it != varTable.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second << endl;
	}
}
