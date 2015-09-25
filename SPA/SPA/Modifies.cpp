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

list<int> Modifies::getModifiesLine( int var_id )
{
	if (modifiesMap.find(var_id) == modifiesMap.end()) {
		return list<int>();
	} else{
		return modifiesMap.at(var_id);
	}
	
}

void Modifies::setModifiesStmt( int var_id, int stmt_number )
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

void Modifies::printMap() {
	for (map<int, std::list<int>>::iterator it = modifiesMap.begin(); it != modifiesMap.end(); ++it) {

		cout << (*it).first;
		cout << ":";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			cout << *listIt;
			cout << " ";
		}
		cout << endl;
	}
}

void Modifies::sortMap() {
	for (map<int, std::list<int>>::iterator it = modifiesMap.begin(); it != modifiesMap.end(); ++it) {
		(*it).second.sort();
	}
}