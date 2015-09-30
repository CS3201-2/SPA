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

	void setParent(int, list<int>);
	int getParentFirst(int);
	list<int> getParentSecond(int);
	bool isParentValid(int, int);
	void printAllParent();

	list<int> getChildStmt(int);

	void setParentStmt(int, list<int>);

	int getParentStmt(int);

	void printParentMap();

	void sortParentMap();

	int getParentMapSize();
private:
	map<int, std::list<int>> parentMap;
};

#endif