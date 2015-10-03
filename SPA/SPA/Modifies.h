#include "SPALog.h"
#include <string>
#include <map>
#include <list>

using namespace std;


#ifndef Modifies_H
#define Modifies_H

class Modifies
{
public:
	Modifies();
    
	void setModifies(int, list<int>);
	list<int> getModifiesFirst(int);
	list<int> getModifiesSecond(int);
	bool isModifiesValid(int, int);
	void logModifies();
	void sortAndUnifyMap();

private:
	map<int, list<int>> modifiesMap;
};

#endif