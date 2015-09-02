#include "QueryValidator.h"
#include <iostream>

int main() {
	QueryValidator q;
	list<string> queries;
	string str = "assign a;variable b; Select a     such that Uses(a, \"x\")";
	queries.push_back(str);
	cout << str << "\n";
	q.isValidQuery(queries);
	cin.ignore();
	cin.get();
	return 0;
}