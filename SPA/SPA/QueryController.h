#pragma once

#include <string>
#include <list>
#include "PKB.h"

using namespace std;

#ifndef QueryController_H
#define QueryController_H

class QueryController
{
private:
public:
	void validateQueries(list<string> queries, PKB my_pkb);

};

#endif

