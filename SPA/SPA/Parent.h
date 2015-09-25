#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef Parent_H
#define Parent_H

class Parent
{
public:
	Parent();

	list<int> getChildStmt(int);

	void setParentStmt(int, int);

	int getParentStmt(int);

	void printParentMap();

	void sortParentMap();

	int getParentMapSize();
private:
	map<int, std::list<int>> parentMap;
};

#endif