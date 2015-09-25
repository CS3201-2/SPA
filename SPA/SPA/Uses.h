#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef Uses_H
#define Uses_H

class Uses
{
public:
	Uses();

	list<int> getUsesStmt(int);

	void setUsesStmt(int, int);

	list<int> getUsesVar(int stmt_number);

	void printMap();

	void sortMap();
private:
	map<int, std::list<int>> usesMap;
};

#endif