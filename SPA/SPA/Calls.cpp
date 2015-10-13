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

list<int> Calls::getCallsFirst(int second) {
	if (callsMapReverse.find(second) == callsMapReverse.end()) {
		return list<int>();
	}
	else {
		return callsMapReverse.at(second);
	}
}

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
	for (map<int, list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		str += procTable.getProcName((*it).first) + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += procTable.getProcName(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "calls table reverse\n";
	for (map<int, list<int>>::iterator it = callsMapReverse.begin(); it != callsMapReverse.end(); ++it) {
		str += procTable.getProcName((*it).first) + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += procTable.getProcName(*it2) + ", ";
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

void Calls::setCallsReverse() {
	for (map<int, list<int>>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			if (callsMapReverse.find(*it2) == callsMapReverse.end()) {
				list<int> temp;
				temp.push_back((*it).first);
				callsMapReverse[*it2] = temp;
			}
			else {
				list<int> temp = callsMapReverse.at(*it2);
				temp.push_back((*it).first);
				callsMapReverse[*it2] = temp;
			}
		}
	}
}