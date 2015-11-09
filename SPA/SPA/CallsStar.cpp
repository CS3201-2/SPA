#include "CallsStar.h"

CallsStar::CallsStar() {
}

void CallsStar::setCallsStar(int first, list<int> second) {
	callsStarMap[first] = second;
}

list<int> CallsStar::getCallsStarFirst(int second) {
	if (callsStarMapReverse.find(second) == callsStarMapReverse.end()) {
		return list<int>();
	}
	else {
		return callsStarMapReverse.at(second);
	}
}

list<int> CallsStar::getCallsStarSecond(int first) {
	if (callsStarMap.find(first) == callsStarMap.end()) {
		return list<int>();
	}
	else {
		return callsStarMap.at(first);
	}
}

bool CallsStar::isCallsStarValid(int first, int second) {
	if (callsStarMap.find(first) == callsStarMap.end()) {
		return false;
	}
	else {
		list<int> callsList = callsStarMap.at(first);
		return find(callsList.begin(), callsList.end(), second) != callsList.end();
	}
}

void CallsStar::logCallsStar(ProcTable procTable) {
	string str = "callsstar table\n";
	for (map<int, list<int>>::iterator it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
		str += procTable.getProcName((*it).first) + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += procTable.getProcName(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "callsstar table reverse\n";
	for (map<int, list<int>>::iterator it = callsStarMapReverse.begin(); it != callsStarMapReverse.end(); ++it) {
		str += procTable.getProcName((*it).first) + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += procTable.getProcName(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void CallsStar::sortAndUnifyMap() {
	for (map<int, std::list<int>>::iterator it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}

map<int, list<int>> CallsStar::getCallsStarMap() {
	return callsStarMap;
}

void CallsStar::setCallsStarReverse() {
	for (map<int, list<int>>::iterator it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (callsStarMapReverse.find(*it2) == callsStarMapReverse.end()) {
				list<int> temp;
				temp.push_back((*it).first);
				callsStarMapReverse[*it2] = temp;
			}
			else {
				list<int> temp = callsStarMapReverse.at(*it2);
				temp.push_back((*it).first);
				callsStarMapReverse[*it2] = temp;
			}
		}
	}
}