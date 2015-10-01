#include "SPALog.h"
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

	void setCalls(int, int);
	list<int> getCallsFirst(int);
	list<int> getCallsSecond(int);
	bool isCallValid(int, int);
	void logCalls();
	
private:
	map<int, list<int>> callsMap;
};

#endif