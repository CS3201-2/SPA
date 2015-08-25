#include "Modifies.h"
#include <map>
#include <list>

using namespace std;

// constructor
Modifies::Modifies()
{
}

list<int> Modifies::get_modifies_line( int var_id )
{
	return modifiesMap.at( var_id );
}

void Modifies::set_modifies_stm( int var_id, int stm_number )
{
	std::list<int> list = modifiesMap.at( var_id );

	list.push_back( stm_number );

	modifiesMap[ var_id ] = list;

}

