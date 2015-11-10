#include "SPALog.h"
#include "ProcTable.h"
#include "VarTable.h"
#include <string>
#include <map>
#include <list>

using namespace std;


#ifndef Uses_H
#define Uses_H

class Uses{

public:
	Uses();

	void setUses(int, list<int>);
	void resetUses(int, list<int>);
	list<int> getUsesFirst(int);
	list<int> getUsesSecond(int);
	bool isUsesValid(int, int);
	void logUses(ProcTable, VarTable);
	void sortAndUnifyMap();
	void setUsesReverse();

private:
	map<int, list<int>> usesMap;
	map<int, list<int>> usesMapReverse;
};

#endif