#include "Parent.h"


// constructor
Parent::Parent(){
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
	if (parentMapReverse.find(second) == parentMapReverse.end()) {
		return 0;
	}
	else {
		return parentMapReverse.at(second);
	}
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
	for (map<int, list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		str += to_string((*it).first) + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += to_string(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "parent table reverse\n";
	for (map<int, int>::iterator it = parentMapReverse.begin(); it != parentMapReverse.end(); ++it) {
		str += to_string((*it).first) + ": " + to_string((*it).second) + "\n";
	}
	str += "\n";

	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

map<int, list<int>> Parent::getParentMap() {
	return parentMap;
}

void Parent::sortAndUnifyMap() {
	for (map<int, list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}

void Parent::setParentReverse() {
	for (map<int, list<int>>::iterator it = parentMap.begin(); it != parentMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			parentMapReverse[*it2] = (*it).first;
		}
	}
}