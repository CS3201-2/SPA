#include "Follows.h"


Follows::Follows() {
}

void Follows::setFollows(int first, int second) {
	followsMap[first] = second;
}

int Follows::getFollowsFirst(int second) {
	for (map<int, int>::iterator it = followsMap.begin(); it != followsMap.end(); ++it) {
		if ((*it).second == second) {
			return (*it).first;
		}
	}

	return 0;
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

void Follows::printFollowsMap() {
	cout << "follows table" << endl;
	for (map<int, int>::iterator it = followsMap.begin(); it != followsMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second;
		cout << endl;
	}
	cout << endl;
}