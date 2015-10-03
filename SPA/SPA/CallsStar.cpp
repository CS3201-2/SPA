#include "CallsStar.h"

CallsStar::CallsStar() {
}

void CallsStar::setCallsStar(int first, int second) {
	list<int> secondList;

	if (callsStarMap.find(first) == callsStarMap.end()) {
		secondList.push_back(second);
		callsStarMap[first] = secondList;
	}
	else {
		secondList = callsStarMap.at(first);
		secondList.push_back(second);
		callsStarMap[first] = secondList;
	}
}

//Calls (B, 8): B will be returned
list<int> CallsStar::getCallsStarFirst(int second) {
	list<int> result;
	for (map<int, list<int>>::iterator it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), second) != (*it).second.end()) {
			result.push_back((*it).first);
		}
	}
	return result;
}

//Calls (8, A), A will be returned
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

void CallsStar::logCallsStar() {
	string str = "callsstar table\n";
	for (map<int, list<int>>::iterator it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
		str += to_string((*it).first) + ": ";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			str += to_string(*listIt) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	SPALog::log(str);
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