#include "SPALog.h"
#include <string>
#include <map>
#include <list>

using namespace std;


#ifndef Parent_H
#define Parent_H

class Parent
{
public:
	Parent();

	void setParent(int, list<int>);
	int getParentFirst(int);
	list<int> getParentSecond(int);
	bool isParentValid(int, int);
	void logParent();
	map<int, list<int>> getParentMap();
	void sortAndUnifyMap();

private:
	map<int, list<int>> parentMap;
};

#endif