#include <string>
#include <map>
#include <list>
#include <iostream>

using namespace std;


#ifndef Modifies_H
#define Modifies_H

class Modifies
{
public:
	Modifies();
    
	void setModifies();
	list<int> getModifiesFirst(int);
	list<int> getModifiesSecond(int);
	bool isModifiesValid(int, int);
	void printAllModifies();

private:
	map<int, list<int>> modifiesMap;
};

#endif