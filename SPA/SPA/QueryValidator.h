#pragma once
#include <string>
#include <list>

using namespace std;

typedef int boolean;
#define true 1
#define false 0

class QueryValidator {
public:
	boolean validateQuery(list<string> query);
};
