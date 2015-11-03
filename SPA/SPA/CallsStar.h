#include "SPALog.h"
#include "ProcTable.h"
#include <string>
#include <map>
#include <list>
#include <algorithm>

using namespace std;


#ifndef CallsStar_H
#define CallsStar_H

class CallsStar{

public:
	CallsStar();

	void setCallsStar(int, list<int>);
	list<int> getCallsStarFirst(int);
	list<int> getCallsStarSecond(int);
	bool isCallsStarValid(int, int);
	void logCallsStar(ProcTable procTable);
	void sortAndUnifyMap();
	map<int, list<int>> getCallsStarMap();
	void setCallsStarReverse();

private:
	map<int, list<int>> callsStarMap;
	map<int, list<int>> callsStarMapReverse;
};

#endif
