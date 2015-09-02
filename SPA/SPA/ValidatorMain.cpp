#include "QueryValidator.h"
#include <iostream>

int main() {
	QueryValidator q;
	list<string> queries;
	string str = "assign a;variable b; Select a     such that Uses(a, \"x\")", //for trim
		str2 = "assign a#1;variable #b; Select a     such that Uses(a, \"x\")",  //for isValidVariableName
		str3 = "Select a     such that Uses(a, \"x\")";  //for no declarations
	queries.push_back(str);
	queries.push_back(str2);
	queries.push_back(str3);
	cout << str << "\n";
	cout << str2 << "\n";
	cout << str3 << "\n";
	q.areValidQueries(queries);
	cin.ignore();
	cin.get();
	cout << "\n out";
	return 0;
}