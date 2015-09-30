#include "Uses.h"

using namespace std;

// constructor
Uses::Uses()
{
}

void Uses::setUses() {

}

list<int> Uses::getUsesFirst(int second) {
	list<int> resultList;
	for (map<int, list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), second) != (*it).second.end()) {
			resultList.push_back((*it).first);
		}
	}

	return resultList;
}

list<int> Uses::getUsesSecond(int first) {
	if (usesMap.find(first) == usesMap.end()) {
		return list<int>();
	}
	else {
		return usesMap.at(first);
	}
}

bool Uses::isUsesValid(int first, int second) {
	if (usesMap.find(first) == usesMap.end()) {
		return false;
	}
	else {
		list<int> childrenList = usesMap.at(first);
		return find(childrenList.begin(), childrenList.end(), second) != childrenList.end();
	}
}

void Uses::printAllUses() {
	cout << "uses table" << endl;
	for (map<int, std::list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			cout << *listIt;
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}










list<int> Uses::getUsesStmt(int var_id)
{
	if (usesMap.find(var_id) == usesMap.end()) {
		return list<int>();
	}
	else {
		return usesMap.at(var_id);
	}
}

void Uses::setUsesStmt(int var_id, int stmt_number)
{
	if (usesMap.find(var_id) == usesMap.end()) {
		std::list<int> emptyList;
		emptyList.push_back(stmt_number);
		usesMap[var_id] = emptyList;
	}
	else {
		std::list<int> list = usesMap.at(var_id);

		if (std::find(list.begin(), list.end(), stmt_number) == list.end()) {
			list.push_back(stmt_number);
		}
		usesMap[var_id] = list;
	}
}

list<int> Uses::getUsesVar(int stmtNumber) {
	list<int> varList;
	for (map<int, std::list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), stmtNumber) != (*it).second.end()) {
			varList.push_back((*it).first);
		}
	}
	return varList;
}

void Uses::printMap() {
	cout << "uses table" << endl;
	for (map<int, std::list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			cout << *listIt;
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Uses::sortMap() {
	for (map<int, std::list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		(*it).second.sort();
	}
}