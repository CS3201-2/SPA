#include "QueryValidator.h"
#include <iostream>

int main() {
	QueryValidator q;
	list<string> queries;
	string str = "assign a, c;variable b; Select a     such that Uses(a, \"x\")", //for trim
		str2 = "assign a#1;variable #b; Select a     such that Uses(a, \"x\")",  //for isValidVariableName
		str3 = "Select a     such that Uses(a, \"x\")",  //for no declarations
		str4 = "assign a,    c; variable b;Select a     such that Uses(a, \"x\")", // for multiple variables 
		str5 = "assign a,    c; variable aa;Select a     such that Uses(a, \"x\")", // to check map
		str6 = "assign a, c; variable b;Select   a     such that Uses(a, \"x\")";
	//queries.push_back(str);
	//queries.push_back(str2);
	//queries.push_back(str3);
	//queries.push_back(str4);
	queries.push_back(str6);
	//cout << str << "\n";
	//cout << str2 << "\n";
	//cout << str3 << "\n";
	//cout << str4 << "\n";
	cout << str6 << "\n";
	q.areValidQueries(queries);
	cin.ignore();
	cin.get();
	cout << "\n out";
	return 0;
}