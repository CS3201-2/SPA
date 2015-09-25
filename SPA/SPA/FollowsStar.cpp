#include "FollowsStar.h"

FollowsStar::FollowsStar() {
}

void FollowsStar::createFollowsStarMap() {
	for (int i = 0; i < follows.getFollowsMapSize; i++) {
		if (follows.getFollowsStmt(i) != -1) {
			followsStarMap.at(i).push_back(follows.getFollowsStmt(i));
			int j = follows.getFollowsStmt(i);
			while (j != -1) {
				followsStarMap.at(i).push_back(follows.getFollowsStmt(j));
				j = follows.getFollowsStmt(j);
			}
		}
	}
}

list<int> FollowsStar::getFollowsStarStmt(int stmtLine) {
	return followsStarMap[stmtLine];
}

list<int> FollowsStar::getIsFollowedStarStmt(int stmtLine) {
	std::list<int> stmtList;
	for (map<int, std::list<int>>::iterator it = followsStarMap.begin(); it != followsStarMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), stmtLine) != (*it).second.end) {
			stmtList.push_back((*it).first);
		}
	}
	stmtList.sort;
	return stmtList;
}

void FollowsStar::printFollowsStarMap() {
	for (map<int, std::list<int>>::iterator it = followsStarMap.begin(); it != followsStarMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			cout << *listIt;
			cout << " ";
		}
		cout << endl;
	}
}