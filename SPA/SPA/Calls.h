#include "SPALog.h"
#include "ProcTable.h"
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
	void logCalls(ProcTable);
	void sortAndUnifyMap();
	map<int, list<int>> getCallsMap();
	void setCallsReverse();

private:
	map<int, list<int>> callsMap;
	map<int, list<int>> callsMapReverse;
};

#endif