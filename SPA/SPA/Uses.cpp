#include "Uses.h"
#include <map>
#include <list>

using namespace std;

// constructor
Uses::Uses()
{
}

list<int> Uses::get_uses_stm( int var_id )
{
	return usesMap.at( var_id );
}

void Uses::set_uses_stm( int var_id, int stm_number )
{
	std::list<int> list = usesMap.at( var_id );

	list.push_back( stm_number );

	usesMap[ var_id ] = list;

}