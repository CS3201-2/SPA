#pragma once
#include <string>
#include <list>
#include <cstdbool>

using namespace std;

class QueryValidator {
private:
	bool parseQuery(list<bool>);
public:
	bool validateQuery(list<string> query);
};
