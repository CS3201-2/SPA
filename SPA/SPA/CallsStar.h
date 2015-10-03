#include "SPALog.h"
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;


#ifndef CallsStar_H
#define CallsStar_H

class CallsStar
{
public:
	CallsStar();

	void setCallsStar(int, int);
	list<int> getCallsStarFirst(int);
	list<int> getCallsStarSecond(int);
	bool isCallsStarValid(int, int);
	bool isCallsStarFirstFound(int);
	void logCallsStar();
	void sortAndUnifyMap();
	map<int, list<int>> getCallsStarMap();

private:
	map<int, list<int>> callsStarMap;
};

#endif
