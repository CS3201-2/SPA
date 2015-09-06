#include "Modifies.h"
#include <map>
#include <list>
#include <algorithm>

using namespace std;

// constructor
Modifies::Modifies()
{
}

list<int> Modifies::get_modifies_line( int var_id )
{
	return modifiesMap.at( var_id );
}

void Modifies::set_modifies_stmt( int var_id, int stmt_number )
{
	std::list<int> list = modifiesMap.at( var_id );

	if (std::find(list.begin, list.end, stmt_number) == list.end()) {
		list.push_back(stmt_number);
		list.sort();
	}

	modifiesMap[ var_id ] = list;

}

