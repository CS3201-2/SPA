#pragma once
#include <string>
#include <list>
#include <cstdbool>

using namespace std;

class QueryValidator {
private:
	bool parseQuery(string query);
	bool parseDeclaration(string declaration);
public:
	list<bool> isValidQuery(list<string> query);
};
