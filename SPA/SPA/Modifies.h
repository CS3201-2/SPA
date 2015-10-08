#include "SPALog.h"
#include "ProcTable.h"
#include "VarTable.h"
#include <string>
#include <map>
#include <list>

using namespace std;


#ifndef Modifies_H
#define Modifies_H

class Modifies{

public:
	Modifies();
    
	void setModifies(int, list<int>);
	void resetModifies(int, list<int>);
	list<int> getModifiesFirst(int);
	list<int> getModifiesSecond(int);
	bool isModifiesValid(int, int);
	void logModifies(ProcTable, VarTable);
	void sortAndUnifyMap();
	void setModifiesReverse();

private:
	map<int, list<int>> modifiesMap;
	map<int, list<int>> modifiesMapReverse;
};

#endif