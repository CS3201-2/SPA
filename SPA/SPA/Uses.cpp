#include "Uses.h"


// constructor
Uses::Uses(){
}

void Uses::setUses(int first, list<int> second) {
	if (second.size() != 0) {
		if (usesMap.find(first) == usesMap.end()) {
			usesMap[first] = second;
		}
		else {
			list<int> existingUsesList = usesMap.at(first);
			existingUsesList.insert(existingUsesList.end(), second.begin(), second.end());
			usesMap[first] = existingUsesList;
		}
	}
}

void Uses::resetUses(int first, list<int> second) {
	usesMap[first] = second;
}

list<int> Uses::getUsesFirst(int second) {
	if (usesMapReverse.find(second) == usesMapReverse.end()) {
		return list<int>();
	}
	else {
		return usesMapReverse.at(second);
	}
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

void Uses::logUses(ProcTable procTable, VarTable varTable) {
	string str = "uses table\n";
	for (map<int, list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		if ((*it).first < 0) {
			str += procTable.getProcName((*it).first) + ": ";
		}
		else {
			str += to_string((*it).first) + ": ";
		}
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += varTable.getVarName(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "uses table reverse\n";
	for (map<int, list<int>>::iterator it = usesMapReverse.begin(); it != usesMapReverse.end(); ++it) {
		str += varTable.getVarName((*it).first) + ": ";

		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (*it2 < 0) {
				str += procTable.getProcName(*it2) + ", ";
			}
			else {
				str += to_string(*it2) + ", ";
			}
		}
		str += "\n";
	}
	str += "\n";

	SPALog::log(str);
}

void Uses::sortAndUnifyMap() {
	for (map<int, list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}

void Uses::setUsesReverse() {
	for (map<int, list<int>>::iterator it = usesMap.begin(); it != usesMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (usesMapReverse.find(*it2) == usesMapReverse.end()) {
				list<int> temp;
				temp.push_back((*it).first);
				usesMapReverse[*it2] = temp;
			}
			else {
				list<int> temp = usesMapReverse.at(*it2);
				temp.push_back((*it).first);
				usesMapReverse[*it2] = temp;
			}
		}
	}
}