#include "Calls.h"

Calls::Calls() {
}

void Calls::setCalls(int first, int second) {
	list<int> secondList;

	if (callsMap.find(first) == callsMap.end()) {
		secondList.push_back(second);
		callsMap[first] = secondList;
	}
	else {
		secondList = callsMap.at(first);
		secondList.push_back(second);
		callsMap[first] = secondList;
	}
}

//Calls (B, 8): B will be returned
list<int> Calls::getCallsFirst(int second) {
	list<int> result;
	for (map<int, list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		if (find((*it).second.begin(), (*it).second.end(), second) != (*it).second.end()) {
			result.push_back((*it).first);
		}
	}
	return result;
}

//Calls (8, A), A will be returned
list<int> Calls::getCallsSecond(int first) {
	if (callsMap.find(first) == callsMap.end()) {
		return list<int>();
	}
	else {
		return callsMap.at(first);
	}
}

bool Calls::isCallValid(int first, int second) {
	if (callsMap.find(first) == callsMap.end()) {
		return false;
	}
	else {
		list<int> callsList = callsMap.at(first);
		return find(callsList.begin(), callsList.end(), second) != callsList.end();
	}
}

void Calls::logCalls(ProcTable procTable) {
	string str = "calls table\n";
	for (map<int, std::list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		str += procTable.getProcName((*it).first) + ": ";
		for (list<int>::iterator listIt = (*it).second.begin(); listIt != (*it).second.end(); ++listIt) {
			str += procTable.getProcName(*listIt) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	SPALog::log(str);
}

map<int, list<int>> Calls::getCallsMap() {
	return callsMap;
}

void Calls::sortAndUnifyMap() {
	for (map<int, std::list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}