#include "Uses.h"
#include <map>
#include <list>
#include <algorithm>

using namespace std;

// constructor
Uses::Uses()
{
}

list<int> Uses::get_uses_stmt( int var_id )
{
	if (usesMap.find(var_id) == usesMap.end()) {
		return list<int>();
	}
	else {
		return usesMap.at(var_id);
	}
}

void Uses::set_uses_stmt( int var_id, int stmt_number )
{
	if (usesMap.find(var_id) == usesMap.end()) {
		std::list<int> emptyList;
		emptyList.push_back(stmt_number);
		usesMap[var_id] = emptyList;
	}
	else {
		std::list<int> list = usesMap.at(var_id);

		if (std::find(list.begin(), list.end(), stmt_number) == list.end()) {
			list.push_back(stmt_number);
		}
		usesMap[var_id] = list;
	}
}

list<int> Uses::getUsesVar(int stmtNumber) {
	list<int> varList;
	for (int i = 0; i < usesMap.size(); i++) {
		if (find(usesMap[i].begin(), usesMap[i].end(), stmtNumber) != usesMap[i].end()) {
			varList.push_back(i);
		}
	}
	return varList;
}