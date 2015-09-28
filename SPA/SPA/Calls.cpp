#include "Calls.h"

Calls::Calls() {
}

void Calls::setCallsStmt(int callsFirst, int callsSecond) {
	callsMap[callsFirst] = callsSecond;
}

//Calls (B, 8): B will be returned
int Calls::getCallsFirst(int callsSecond) {
	int result = 0;
	for (map<int, int>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		if ((*it).second == callsSecond) {
			result = (*it).first;
			break;
		}
	}
	return result;
}

//Calls (8, A), A will be returned
int Calls::getCallsSecond(int callsFirst) {
	if (callsMap.find(callsFirst) == callsMap.end()) {
		return 0;
	}
	else {
		return callsMap.at(callsFirst);
	}
}

bool Calls::isCallValid(int callsFirst, int callsSecond) {
	if (callsMap.find(callsFirst) == callsMap.end()) {
		return false;
	}
	else {
		return callsMap.at(callsFirst) == callsSecond;
	}
}
void Calls::printCallsMap() {
	cout << "calls table" << endl;
	for (map<int, int>::iterator it = callsMap.begin(); it != callsMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second;
		cout << endl;
	}
	cout << endl;
}