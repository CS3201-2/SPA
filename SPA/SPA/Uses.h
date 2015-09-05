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

	list<int> get_uses_stmt( int );

	void set_uses_stmt( int, int );

private:
	map<int,std::list<int>> usesMap;
};

#endif