#include "Follows.h"


Follows::Follows() {
}

void Follows::setFollows(int first, int second) {
	followsMap[first] = second;
}

int Follows::getFollowsFirst(int second) {
	if (followsMapReverse.find(second) == followsMapReverse.end()) {
		return 0;
	}
	else {
		return followsMapReverse.at(second);
	}
}

int Follows::getFollowsSecond(int first) {
	if (followsMap.find(first) == followsMap.end()) {
		return 0;
	}
	else {
		return followsMap.at(first);
	}
}

bool Follows::isFollowValid(int first, int second) {
	if (followsMap.find(first) == followsMap.end()) {
		return false;
	}
	else {
		return followsMap.at(first) == second;
	}
}

void Follows::logFollows() {
	string str = "follows table\n";
	for (map<int, int>::iterator it = followsMap.begin(); it != followsMap.end(); ++it) {
		str += to_string((*it).first) + ": " + to_string((*it).second) + "\n";
	}
	str += "\n";

	str += "follows reverse table\n";
	for (map<int, int>::iterator it = followsMapReverse.begin(); it != followsMapReverse.end(); ++it) {
		str += to_string((*it).first) + ": " + to_string((*it).second) + "\n";
	}
	str += "\n";

	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

map<int, int> Follows::getFollowsMap() {
	return followsMap;
}

void Follows::setFollowsReverse() {
	for (map<int, int>::iterator it = followsMap.begin(); it != followsMap.end(); ++it) {
		followsMapReverse[(*it).second] = (*it).first;
	}
}