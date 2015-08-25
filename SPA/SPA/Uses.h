#include <string>
#include <map>
#include <list>

using namespace std;

#ifndef Uses_H
#define Uses_H

class Uses
{
public:
	Uses();

	list<int> get_uses_stm( int );

	void set_uses_stm( int, int );

private:
	map<int,std::list<int>> usesMap;
};

#endif