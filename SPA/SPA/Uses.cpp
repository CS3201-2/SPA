#include "Uses.h"


// constructor
Uses::Uses()
{
}

// need to see parser to decide the input
void Uses::setUses(int first, list<int> second) {
	if (usesMap.find(first) == usesMap.end()) {
		usesMap[first] = second;
	}
	else {
		list<int> existingUsesList = usesMap.at(first);
		existingUsesList.insert(existingUsesList.end(), second.begin(), second.end());
		usesMap[first] = existingUsesList;
	}
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
		list<int> usesList = usesMap.at(first);
		return find(usesList.begin(), usesList.end(), second) != usesList.end();
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








/*void Uses::sortMap() {
	for (map<int, std::list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		(*it).second.sort();
	}
}*/