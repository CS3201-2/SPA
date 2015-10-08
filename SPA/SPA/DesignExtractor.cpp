#include "DesignExtractor.h"


DesignExtractor::DesignExtractor() {
}

void DesignExtractor::setReverseMap() {
	PKB::getPKBInstance()->setModifiesReverse();
	PKB::getPKBInstance()->setUsesReverse();
	PKB::getPKBInstance()->setFollowsReverse();
	PKB::getPKBInstance()->setFollowsStarReverse();
	PKB::getPKBInstance()->setParentReverse();
	PKB::getPKBInstance()->setParentStarReverse();
	PKB::getPKBInstance()->setCallsReverse();
	PKB::getPKBInstance()->setCallsStarReverse();
	PKB::getPKBInstance()->setVarTableReverse();
	PKB::getPKBInstance()->setProcTableReverse();
}

void DesignExtractor::setCalls(list<pair<int, string>> calledProcList) {
	for (list<pair<int, string>>::iterator it = calledProcList.begin(); it != calledProcList.end(); ++it) {
		int callFirstID = (*it).first;
		int callSecondID = PKB::getPKBInstance()->getProcID((*it).second);
		PKB::getPKBInstance()->setCalls(callFirstID, callSecondID);
	}

	PKB::getPKBInstance()->sortAndUnifyCallsMap();
}

void DesignExtractor::setCallsStar() {
	map<int, list<int>> callsMap = PKB::getPKBInstance()->getCallsMap();
	bool isValid = true;

	//loop through all the first in callsMap
	for (map<int, list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		int first = (*it).first;
		if (PKB::getPKBInstance()->getCallsStarSecond(first).size() == 0) {
			list<int> firstList;
			firstList.push_back(first);
			processCallsStar(isValid, firstList);
		}
	}

	PKB::getPKBInstance()->sortAndUnifyCallsStarMap();
}

void DesignExtractor::processCallsStar(bool& isValid, list<int> firstList) {
	int first = firstList.back();
	list<int> secondList = PKB::getPKBInstance()->getCallsSecond(first);

	for (list<int>::iterator it = secondList.begin(); it != secondList.end(); ++it) {
		list<int> tempFirstList = firstList;
		if (find(tempFirstList.begin(), tempFirstList.end(), *it) == tempFirstList.end()) {
			for (list<int>::iterator it2 = tempFirstList.begin(); it2 != tempFirstList.end(); ++it2) {
				PKB::getPKBInstance()->setCallsStar(*it2, *it);
			}
		}
		else {
			isValid = false;
			return;
		}
		tempFirstList.push_back(*it);
		processCallsStar(isValid, tempFirstList);
	}
}

void DesignExtractor::setFollowsStar() {
	map<int, int> followsMap = PKB::getPKBInstance()->getFollowsMap();
	list<int> tempList;

	for (map<int, int>::reverse_iterator it = followsMap.rbegin(); it != followsMap.rend(); ++it) {
		int first = (*it).first;
		int firstPotential = (*it).second;

		tempList.push_back(firstPotential);
		list<int> secondList = PKB::getPKBInstance()->getFollowsStarSecond(firstPotential);
		tempList.insert(tempList.end(), secondList.begin(), secondList.end());

		PKB::getPKBInstance()->setFollowsStar(first, tempList);
		tempList.clear();
	}
}

void DesignExtractor::setParentStar() {
	map<int, list<int>> parentMap = PKB::getPKBInstance()->getParentMap();
	list<int> tempList;
	for (map<int, list<int>>::reverse_iterator it = parentMap.rbegin(); it != parentMap.rend(); ++it) {
		int first = (*it).first;
		list<int> potentialFirstList = (*it).second;

		for (list<int>::iterator it2 = potentialFirstList.begin(); it2 != potentialFirstList.end(); ++it2) {
			tempList.push_back(*it2);
			list<int> secondList = PKB::getPKBInstance()->getParentStarSecond(*it2);
			tempList.insert(tempList.end(), secondList.begin(), secondList.end());
		}

		PKB::getPKBInstance()->setParentStar(first, tempList);
		tempList.clear();
	}
}

void DesignExtractor::resetModifies() {
	resetModifiesForProc();
	resetModifiesForStmt();
}

void DesignExtractor::resetModifiesForProc() {
	map<int, list<int>> procModifiesMap;
	list<int> procList = PKB::getPKBInstance()->getProcList();
	bool isDone;

	for (list<int>::iterator it = procList.begin(); it != procList.end(); ++it) {
		procModifiesMap[*it] = PKB::getPKBInstance()->getModifiesSecond(*it);
	}

	do {
		isDone = true;

		for (map<int, list<int>>::iterator it = procModifiesMap.begin(); it != procModifiesMap.end(); ++it) {
			bool isAllPositive = true;

			for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
				if (*it2 < 0) {
					isAllPositive = false;
					break;
				}
			}

			if (isAllPositive) {
				int first = (*it).first;
				list<int> secondList = (*it).second;
				//adjust the whole map
				modifyProcModifiesMap(procModifiesMap, first, secondList);
			}
		}

		//check isDone
		for (map<int, list<int>>::iterator it = procModifiesMap.begin(); it != procModifiesMap.end(); ++it) {
			int secondListFirstID = (*(*it).second.begin());
			if (secondListFirstID < 0) {
				isDone = false;
				break;
			}
		}
	} while (!isDone);

	//set procModifiesMap back to PKB modifies
	for (map<int, list<int>>::iterator it = procModifiesMap.begin(); it != procModifiesMap.end(); ++it) {
		PKB::getPKBInstance()->resetModifies((*it).first, (*it).second);
	}
}

void DesignExtractor::modifyProcModifiesMap(map<int, list<int>>& procModifiesMap, int first, list<int> secondList) {
	for (map<int, list<int>>::iterator it = procModifiesMap.begin(); it != procModifiesMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (*it2 > first) {
				break;
			}
			if (*it2 == first) {
				list<int> existingSecondList;
				existingSecondList = procModifiesMap.at((*it).first);
				existingSecondList.remove(first);
				existingSecondList.insert(existingSecondList.end(), secondList.begin(), secondList.end());
				existingSecondList.sort();
				existingSecondList.unique();
				procModifiesMap[(*it).first] = existingSecondList;
				break;
			}
		}
	}
}

void DesignExtractor::resetModifiesForStmt() {
	list<int> stmtList = PKB::getPKBInstance()->getStmtList();

	for (list<int>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		list<int> existingSecondList = PKB::getPKBInstance()->getModifiesSecond(*it);
		list<int> tempSecondList = existingSecondList;

		for (list<int>::iterator it2 = existingSecondList.begin(); it2 != existingSecondList.end(); ++it2) {
			if (*it2 < 0) {
				list<int> procSecondList = PKB::getPKBInstance()->getModifiesSecond(*it2);
				tempSecondList.remove(*it2);
				tempSecondList.insert(tempSecondList.end(), procSecondList.begin(), procSecondList.end());
				tempSecondList.sort();
				tempSecondList.unique();
				PKB::getPKBInstance()->resetModifies(*it, tempSecondList);
			}
			else {
				break;
			}
		}
	}
}

void DesignExtractor::resetUses() {
	resetUsesForProc();
	resetUsesForStmt();
}

void DesignExtractor::resetUsesForProc() {
	map<int, list<int>> procUsesMap;
	list<int> procList = PKB::getPKBInstance()->getProcList();
	bool isDone;

	for (list<int>::iterator it = procList.begin(); it != procList.end(); ++it) {
		procUsesMap[*it] = PKB::getPKBInstance()->getUsesSecond(*it);
	}

	do {
		isDone = true;

		for (map<int, list<int>>::iterator it = procUsesMap.begin(); it != procUsesMap.end(); ++it) {
			bool isAllPositive = true;

			for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
				if (*it2 < 0) {
					isAllPositive = false;
					break;
				}
			}

			if (isAllPositive) {
				int first = (*it).first;
				list<int> secondList = (*it).second;
				//adjust the whole map
				modifyProcUsesMap(procUsesMap, first, secondList);
			}
		}

		//check isDone
		for (map<int, list<int>>::iterator it = procUsesMap.begin(); it != procUsesMap.end(); ++it) {
			int secondListFirstID = (*(*it).second.begin());
			if (secondListFirstID < 0) {
				isDone = false;
				break;
			}
		}
	} while (!isDone);

	//set procUsesMap back to PKB uses
	for (map<int, list<int>>::iterator it = procUsesMap.begin(); it != procUsesMap.end(); ++it) {
		PKB::getPKBInstance()->resetUses((*it).first, (*it).second);
	}
}

void DesignExtractor::modifyProcUsesMap(map<int, list<int>>& procUsesMap, int first, list<int> secondList) {
	for (map<int, list<int>>::iterator it = procUsesMap.begin(); it != procUsesMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (*it2 > first) {
				break;
			}
			if (*it2 == first) {
				list<int> existingSecondList;
				existingSecondList = procUsesMap.at((*it).first);
				existingSecondList.remove(first);
				existingSecondList.insert(existingSecondList.end(), secondList.begin(), secondList.end());
				existingSecondList.sort();
				existingSecondList.unique();
				procUsesMap[(*it).first] = existingSecondList;
				break;
			}
		}
	}
}

void DesignExtractor::resetUsesForStmt() {
	list<int> stmtList = PKB::getPKBInstance()->getStmtList();

	for (list<int>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		list<int> existingSecondList = PKB::getPKBInstance()->getUsesSecond(*it);
		list<int> tempSecondList = existingSecondList;

		for (list<int>::iterator it2 = existingSecondList.begin(); it2 != existingSecondList.end(); ++it2) {
			if (*it2 < 0) {
				list<int> procSecondList = PKB::getPKBInstance()->getUsesSecond(*it2);
				tempSecondList.remove(*it2);
				tempSecondList.insert(tempSecondList.end(), procSecondList.begin(), procSecondList.end());
				tempSecondList.sort();
				tempSecondList.unique();
				PKB::getPKBInstance()->resetUses(*it, tempSecondList);
			}
			else {
				break;
			}
		}
	}
}
