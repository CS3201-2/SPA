#include "Calls.h"

Calls::Calls() {
}

void Calls::setCalls(int first, int second) {
	callsMap[first] = second;
}

//Calls (B, 8): B will be returned
int Calls::getCallsFirst(int second) {
	int result = 0;
	for (map<int, int>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		if ((*it).second == second) {
			result = (*it).first;
			break;
		}
	}
	return result;
}

//Calls (8, A), A will be returned
int Calls::getCallsSecond(int first) {
	if (callsMap.find(first) == callsMap.end()) {
		return 0;
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
		return callsMap.at(first) == second;
	}
}
void Calls::printAllCalls() {
	cout << "calls table" << endl;
	for (map<int, int>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second;
		cout << endl;
	}
	cout << endl;
}