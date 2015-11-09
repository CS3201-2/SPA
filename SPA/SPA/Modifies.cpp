#include "Modifies.h"


// constructor
Modifies::Modifies(){
}

void Modifies::setModifies(int first, list<int> second) {
	if (second.size() != 0) {
		if (modifiesMap.find(first) == modifiesMap.end()) {
			modifiesMap[first] = second;
		}
		else {
			list<int> existingModifiesList = modifiesMap.at(first);
			existingModifiesList.insert(existingModifiesList.end(), second.begin(), second.end());
			modifiesMap[first] = existingModifiesList;
		}
	}
}

void Modifies::resetModifies(int first, list<int> second) {
	modifiesMap[first] = second;
}

list<int> Modifies::getModifiesFirst(int second) {
	if (modifiesMapReverse.find(second) == modifiesMapReverse.end()) {
		return list<int>();
	}
	else {
		return modifiesMapReverse.at(second);
	}
}

list<int> Modifies::getModifiesSecond(int first) {
	if (modifiesMap.find(first) == modifiesMap.end()) {
		return list<int>();
	}
	else {
		return modifiesMap.at(first);
	}
}

bool Modifies::isModifiesValid(int first, int second) {
	if (modifiesMap.find(first) == modifiesMap.end()) {
		return false;
	}
	else {
		list<int> modifiesList = modifiesMap.at(first);
		return find(modifiesList.begin(), modifiesList.end(), second) != modifiesList.end();
	}
}

void Modifies::logModifies(ProcTable procTable, VarTable varTable) {
	string str = "modifies table\n";
	for (map<int, list<int>>::iterator it = modifiesMap.begin(); it != modifiesMap.end(); ++it) {
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

	str += "modifies table reverse\n";
	for (map<int, list<int>>::iterator it = modifiesMapReverse.begin(); it != modifiesMapReverse.end(); ++it) {
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

	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void Modifies::sortAndUnifyMap() {
	for (map<int, list<int>>::iterator it = modifiesMap.begin(); it != modifiesMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}

void Modifies::setModifiesReverse() {
	for (map<int, list<int>>::iterator it = modifiesMap.begin(); it != modifiesMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (modifiesMapReverse.find(*it2) == modifiesMapReverse.end()) {
				list<int> temp;
				temp.push_back((*it).first);
				modifiesMapReverse[*it2] = temp;
			}
			else {
				list<int> temp = modifiesMapReverse.at(*it2);
				temp.push_back((*it).first);
				modifiesMapReverse[*it2] = temp;
			}
		}
	}
}