#include "ParentStar.h"


ParentStar::ParentStar() {
}

// need to see design extractor to see how to do this part, might be different from followsStar
void ParentStar::setParentStar(int first, list<int> second) {
	parentStarMap[first] = second;
}

list<int> ParentStar::getParentStarFirst(int second) {
	list<int> resultList;
	for (map<int, list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), second) != (*it).second.end()) {
			resultList.push_back((*it).first);
		}
	}

	return resultList;
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
	for (map<int, std::list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		str += to_string((*it).first) + ": ";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			str += to_string(*listIt) + ", ";
		}
		str += "\n";
	}
	str += "\n";
	SPALog::log(str);
}

void ParentStar::sortAndUnifyMap() {
	for (map<int, std::list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}