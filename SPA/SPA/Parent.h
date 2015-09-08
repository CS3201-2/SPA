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

	int getParentStmt(int);

	void setParentStmt(int, int);

private:
	//key is child id, value is parent id
	map<int, int> parentMap;
};

#endif
