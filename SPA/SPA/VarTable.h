#include <string>
#include <map>

using namespace std;


#ifndef VarTable_H
#define VarTable_H

class VarTable
{
public:
	VarTable();

	int getID(string);

	int insertVar(string);

	int getSize();

	string getVarName(int);

	map<string, int> varTable;

	void printMap();
private:

};

#endif
