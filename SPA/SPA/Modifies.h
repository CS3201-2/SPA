#include <string>
#include <map>
#include <list>

using namespace std;

#ifndef Modifies_H
#define Modifies_H

class Modifies
{
public:
	Modifies();
    
	list<int> get_modifies_line( int );

	void set_modifies_stmt( int, int );

private:
	map<int,std::list<int>> modifiesMap;
};

#endif
