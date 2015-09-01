#pragma once
#include <string>
#include <list>
#include <cstdbool>
#include <vector>

using namespace std;

class QueryValidator {
private:
	bool parseQuery(string query);
	bool parseDeclaration(string declaration);
	vector<string> split(string str, char c);
public:
	list<bool> isValidQuery(list<string> query);

};
