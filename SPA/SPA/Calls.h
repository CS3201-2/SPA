#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef Calls_H
#define Calls_H

class Calls
{
public:
	Calls();

	void setCallsStmt(int, int);

	//Calls (B, 8): B will be returned
	int getCallsFirst(int);

	//Calls (8, A), A will be returned
	int getCallsSecond(int);

	bool isCallValid(int, int);

	void printCallsMap();

private:
	map<int, int> callsMap;
};

#endif