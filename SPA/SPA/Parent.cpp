#include "Parent.h"

using namespace std;

// constructor
Parent::Parent()
{
}

list<int> Parent::getChildStmt(int parent)
{
	if (parentMap.find(parent) == parentMap.end()) {
		return list<int>();
	}
	else {
		return parentMap.at(parent);
	}
}

void Parent::setParentStmt(int parent, list<int> childrenList)
{
	if (parentMap.find(parent) == parentMap.end()) {
		parentMap[parent] = childrenList;
	}
	else {
		list<int> existingChildrenList = parentMap.at(parent);
		existingChildrenList.insert(existingChildrenList.end(), childrenList.begin(), childrenList.end());
		parentMap[parent] = existingChildrenList;
	}
/*
	if (parentMap.find(parent) == parentMap.end()) {
		std::list<int> emptyList;
		emptyList.push_back(child);
		parentMap[parent] = emptyList;
	}
	else {
		std::list<int> list = parentMap.at(parent);

		if (std::find(list.begin(), list.end(), child) == list.end()) {
			list.push_back(child);
		}
		parentMap[parent] = list;
	}*/
}

int Parent::getParentStmt(int stmtNumber) {
	for (map<int, std::list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), stmtNumber) != (*it).second.end()) {
			return (*it).first;
		}
	}
	return -1;
}

void Parent::printParentMap() {
	cout << "parent table" << endl;
	for (map<int, std::list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
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

void Parent::sortParentMap() {
	for (map<int, std::list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}

int Parent::getParentMapSize() {
	return parentMap.size();
}