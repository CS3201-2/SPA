#include "Follows.h"

Follows::Follows() {
}

void Follows::setFollowsStmt(int followsFirst, int followsSecond) {
	followsMap[followsFirst] = followsSecond;
}

int Follows::getFollowsStmt(int stmtLine) {
	if (followsMap.find(stmtLine) == followsMap.end()) {
		return -1;
	}
	else {
		return followsMap.at(stmtLine);
	}
}

int Follows::getIsFollowedStmt(int stmtLine) {
	int result = -1;
	for (map<int, int>::iterator it = followsMap.begin(); it != followsMap.end(); ++it) {
		if ((*it).second == stmtLine) {
			result = (*it).first;
			break;
		}
	}
	return result;
}

int Follows::getFollowsMapSize() {
	return followsMap.size();
}

void Follows::printFollowsMap() {
	for (map<int, int>::iterator it = followsMap.begin(); it != followsMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second;
		cout << endl;
	}
}