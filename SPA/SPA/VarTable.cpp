#include "VarTable.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;

// construactor
VarTable::VarTable(void)
{
}

int VarTable::get_ID(string varName)
{
	if (varTable.find(varName) != varTable.end()) {
		return varTable.at(varName); 
	}
	else {
		return insert_Var(varName);
	}
}

int VarTable::insert_Var(string varName)
{
	int id = varTable.size() + 1;
	varTable.insert( std::pair<string,int>( varName, id ) );
	return id;
}

int VarTable::get_size()
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


