#include "QueryValidator.h"
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
		str8 = "assign   a,   c;Select   a     pattern a (\"a\", _\"x\"_) such that Uses  (a, \"x\") pattern a (\"a\", _\"x\"_)", // to check findSuchThatClause() and findPattern()
		str9 = "assign a; Select a Pattern a(\"x\", \"0\")",
		str10 = "stmt s; Select s such that",
		str11 = "stmt s; Select s such that Modifies(s, \"i\")",
		str12 = "stmt s; Select s such that Next(s,s)",
		str13 = "while w; Select w Pattern w(\"x\", _ )",
		str14 = "if If1; Select If1 Pattern If1(\"i\", _, _ )",
		str15 = "assign a1; assign a2; Select a1 such that Follows(a1, a2)",
		str16 = "assign a; Select a pattern a(\"x\", _)",
		str17 = " ",
		str18 = "assign a; variable v; Select a pattern a(v, _)",
		str19 = "constant c; stmt s; Select s with s.stmt# = c.value",
		str20 = "procedure p; Select p with p.procName = \"SUN\"";

	//cout << str << "\n";

	q.isValidDecAndQuery(str20);

	//RelationshipTable r;
	/*string str18 = "Follows(s, 1)";
	char c = '(';
	vector<string> result;
	const char *strChar = str18.c_str();

	do {
		const char *begin = strChar;

		while (*strChar != c && *strChar) {
			strChar++;
		}

		result.push_back(string(begin, strChar));
	} while (0 != *strChar++);

	for (int i = 0; i < result.size(); i++) {
		cout << result.at(i)<<"\n";
	}*/
	cin.ignore();
	cin.get();
	return 0;
}