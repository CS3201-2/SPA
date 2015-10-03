#include "Parent.h"


// constructor
Parent::Parent()
{
}

void Parent::setParent(int first, list<int> second) {
	if (parentMap.find(first) == parentMap.end()) {
		parentMap[first] = second;
	}
	else {
		list<int> existingChildrenList = parentMap.at(first);
		existingChildrenList.insert(existingChildrenList.end(), second.begin(), second.end());
		parentMap[first] = existingChildrenList;
	}
}

int Parent::getParentFirst(int second) {
	for (map<int, list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), second) != (*it).second.end()) {
			return (*it).first;
		}
	}

	return 0;
}

list<int> Parent::getParentSecond(int first) {
	if (parentMap.find(first) == parentMap.end()) {
		return list<int>();
	}
	else {
		return parentMap.at(first);
	}
}

bool Parent::isParentValid(int first, int second) {
	if (parentMap.find(first) == parentMap.end()) {
		return false;
	}
	else {
		list<int> childrenList = parentMap.at(first);
		return find(childrenList.begin(), childrenList.end(), second) != childrenList.end();
	}
}

void Parent::logParent() {
	string str = "parent table\n";
	for (map<int, std::list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		str += to_string((*it).first) + ": ";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			str += to_string(*listIt) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	SPALog::log(str);
}

map<int, list<int>> Parent::getParentMap() {
	return parentMap;
}

void Parent::sortAndUnifyMap() {
	for (map<int, std::list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}