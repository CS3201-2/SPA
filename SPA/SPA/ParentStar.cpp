#include "ParentStar.h"


ParentStar::ParentStar() {
}

// need to see design extractor to see how to do this part, might be different from followsStar
void ParentStar::setParentStar(int first, list<int> second) {
	parentStarMap[first] = second;
}

list<int> ParentStar::getParentStarFirst(int second) {
	if (parentStarMapReverse.find(second) == parentStarMapReverse.end()) {
		return list<int>();
	}
	else {
		return parentStarMapReverse.at(second);
	}
}

list<int> ParentStar::getParentStarSecond(int first) {
	if (parentStarMap.find(first) == parentStarMap.end()) {
		return list<int>();
	}
	else {
		return parentStarMap.at(first);
	}
}

bool ParentStar::isParentStarValid(int first, int second) {
	if (parentStarMap.find(first) == parentStarMap.end()) {
		return false;
	}
	else {
		list<int> parentList = parentStarMap.at(first);
		return find(parentList.begin(), parentList.end(), second) != parentList.end();
	}
}

void ParentStar::logParentStar() {
	string str = "parent star\n";
	for (map<int, list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		str += to_string((*it).first) + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += to_string(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "parent star Reverse\n";
	for (map<int, list<int>>::iterator it = parentStarMapReverse.begin(); it != parentStarMapReverse.end(); ++it) {
		str += to_string((*it).first) + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += to_string(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	SPALog::log(str);
}

void ParentStar::sortAndUnifyMap() {
	for (map<int, list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}

void ParentStar::setParentStarReverse() {
	for (map<int, list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (parentStarMapReverse.find(*it2) == parentStarMapReverse.end()) {
				list<int> temp;
				temp.push_back((*it).first);
				parentStarMapReverse[*it2] = temp;
			}
			else {
				list<int> temp = parentStarMapReverse.at(*it2);
				temp.push_back((*it).first);
				parentStarMapReverse[*it2] = temp;
			}
		}
	}
}