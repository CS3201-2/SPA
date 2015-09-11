#include <string>
#include <map>

using namespace std;


#ifndef VarTable_H
#define VarTable_H

class VarTable
{
public:
	VarTable();

	int get_ID(string);

	int insert_Var(string);

	int get_size();

	string getVarName(int);

private:
	map<string,int> varTable;
};

#endif
