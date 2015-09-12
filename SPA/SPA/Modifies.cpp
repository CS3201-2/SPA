#include "Modifies.h"
#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

// constructor
Modifies::Modifies()
{
}

list<int> Modifies::get_modifies_line( int var_id )
{
	if (modifiesMap.find(var_id) == modifiesMap.end()) {
		return list<int>();
	} else{
		return modifiesMap.at(var_id);
	}
	
}

void Modifies::set_modifies_stmt( int var_id, int stmt_number )
{
	if (modifiesMap.find(var_id) == modifiesMap.end()) {
		std::list<int> emptyList;
		emptyList.push_back(stmt_number);
		modifiesMap[var_id] = emptyList;
	}
	else {
		std::list<int> list = modifiesMap.at(var_id);

		if (std::find(list.begin(), list.end(), stmt_number) == list.end()) {
			list.push_back(stmt_number);
		}
		modifiesMap[var_id] = list;
	}
}

list<int> Modifies::getModifiesVar(int stmtNumber) {
	list<int> varList;
	for (int i = 0; i < modifiesMap.size(); i++) {
		if (find(modifiesMap[i].begin(), modifiesMap[i].end(), stmtNumber) != modifiesMap[i].end()) {
			varList.push_back(i);
		}
	}
	return varList;
}

