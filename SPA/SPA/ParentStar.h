#include <string>
#include <list>
#include <algorithm>
#include <map>
#include <iostream>
#include "Parent.h"

using namespace std;

class ParentStar 
{
public:
	ParentStar();
	list<int> getChildStarStmt(int);
	list<int> getParentStarStmt(int);
	void createParentStarMap();
	void printParentStarMap();

private:
	Parent parent = Parent();
	map<int, list<int>> parentStarMap;
};