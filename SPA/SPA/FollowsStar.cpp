#include "FollowsStar.h"


FollowsStar::FollowsStar() {
}

void FollowsStar::setFollowsStar(int first, list<int> second) {
	followsStarMap[first] = second;
}

list<int> FollowsStar::getFollowsStarFirst(int second) {
	list<int> resultList;
	for (map<int, list<int>>::iterator it = followsStarMap.begin(); it != followsStarMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), second) != (*it).second.end()) {
			resultList.push_back((*it).first);
		}
	}

	return resultList;
}


list<int> FollowsStar::getFollowsStarSecond(int first) {
	if (followsStarMap.find(first) == followsStarMap.end()) {
		return list<int>();
	}
	else {
		return followsStarMap.at(first);
	}
}

bool FollowsStar::isFollowsStarValid(int first, int second) {
	if (followsStarMap.find(first) == followsStarMap.end()) {
		return false;
	}
	else {
		list<int> followsList = followsStarMap.at(first);
		return find(followsList.begin(), followsList.end(), second) != followsList.end();
	}
}

void FollowsStar::logFollowsStar() {
	string str = "follows star\n";
	for (map<int, std::list<int>>::iterator it = followsStarMap.begin(); it != followsStarMap.end(); ++it) {
		str += to_string((*it).first) + ": ";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			str += to_string(*listIt) + ", ";
		}
		str += "\n";
	}
	str += "\n";
	SPALog::log(str);
}

void FollowsStar::sortAndUnifyMap() {
	for (map<int, std::list<int>>::iterator it = followsStarMap.begin(); it != followsStarMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}