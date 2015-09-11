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
	// return list of var_id modified by stmt number
	list<int> getModifiesVar(int stmt_number);

private:
	map<int,std::list<int>> modifiesMap;
};

#endif
