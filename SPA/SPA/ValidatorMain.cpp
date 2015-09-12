/*#include "QueryValidator.h"
#include "RelationshipTable.h"
#include <iostream>

int main() {
	QueryValidator q;
	list<string> queries;
	string str = "assign a, c;variable b; Select a     \nsuch that Uses(a, \"x\")", //for trim()
		str2 = "assign a#1;variable #b; Select a     such that Uses(a, \"x\")",  //for isValidVariableName()
		str3 = "Select a     such that Uses(a, \"x\")",  //for no declarations
		str4 = "assign a,    c; variable b;Select a     such that Uses(a, \"x\")", // for multiple variables 
		str5 = "assign a,    c; variable aa;Select a     such that Uses(a, \"x\")", // to check map
		str6 = "assign   a,   c;Select   a     such that uses  (_, \"x\")", //to check parseQuery(), findSuchThatClause()
		str7 = "assign   a,   c;Select   a     pattern a (\"a\", _\"x\"_)", //to check parseQuery(), findPattern()
		str8 = "assign   a,   c;Select   a     pattern a (\"a\", _\"x\"_) such that Uses  (_, \"x\") ", // to check findSuchThatClause() and findPattern()
		str9 = "assign a; Select a Pattern a(\"x\", \"0\")";
																										//queries.push_back(str);
	//queries.push_back(str2);
	//queries.push_back(str3);
	//queries.push_back(str4);
	queries.push_back(str8);
	//cout << str << "\n";
	//cout << str2 << "\n";
	//cout << str3 << "\n";
	//cout << str4 << "\n";
	//cout << str6 << "\n";
	q.isValidDecAndQuery(str9);

	//RelationshipTable r;

	cin.ignore();
	cin.get();
	return 0;
}*/