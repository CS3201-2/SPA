#include <string>
#include <map>
#include <list>
#include <iostream>

using namespace std;


#ifndef Uses_H
#define Uses_H

class Uses
{
public:
	Uses();

	void setUses(int, list<int>); //input parameter to be decided later
	list<int> getUsesFirst(int);
	list<int> getUsesSecond(int);
	bool isUsesValid(int, int);
	void printAllUses();
	void sortAndUnifyMap();

private:
	map<int, list<int>> usesMap;
};

#endif