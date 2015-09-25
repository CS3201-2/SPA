#include "ParentStar.h"

using namespace std;

ParentStar::ParentStar() {}

void ParentStar::createParentStarMap() {
	for (int i = 0; i < parent.getParentMapSize; i++) {
		if (!parent.getChildStmt(i).empty) {
			parentStarMap.at(i).insert(parentStarMap.at(i).end, parent.getChildStmt(i).begin, parent.getChildStmt(i).end);
		}
	}
	
	for (int j = 0; j < parentStarMap.size; j++) {
		if (!parentStarMap.at(j).empty) {
			int k = 0;
			while (k < parentStarMap.at(j).size) {
				list<int> childStmt = parentStarMap.at(j);
				for (int m = 0;m < k; m++) {
					childStmt.pop_front;
				}
				int t = childStmt.pop_front;
				if (!parent.getChildStmt(k).empty) {
					parentStarMap.at(j).insert(parentStarMap.at(j).end, parent.getChildStmt(t).begin, parent.getChildStmt(t).end);
				}
				k++;
			}
		}
	}
}

list<int> ParentStar::getChildStarStmt(int parent) {
	if (parentStarMap.find(parent) == parentStarMap.end()) {
		return list<int>();
	}
	else {
		return parentStarMap.at(parent);
	}
}

list<int> ParentStar::getParentStarStmt(int stmtLine) {
	std::list<int> stmtList;
	for (map<int, std::list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), stmtLine) != (*it).second.end()) {
			stmtList.push_back((*it).first);
		}
	}
	stmtList.sort;
	return stmtList;
}

void ParentStar::printParentStarMap() {
	for (map<int, std::list<int>>::iterator it = parentStarMap.begin(); it != parentStarMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			cout << *listIt;
			cout << " ";
		}
		cout << endl;
	}
}