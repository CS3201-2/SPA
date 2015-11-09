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
		str20 = "procedure 1p; Select 1p with 1p.procName = \"SUN\"",
		str21 = "while w; assign a; Select w such that Follows(w, a)",
		str22 = "assign a; while w; variable v; Select a such that Modifies(a, v) pattern w(v, _ , _",
		str23 = "assign a; variable v; Select v such that Modifies(a, v) pattern a(_,\"z + x + i\")",
		str24 = "variable v; assign a; while w;Select <v, a, w> such that Modifies(1,v)",
		str25 = "assign a; Select a pattern a(_, _\"(f - d + b) -- l\"_)",
		str26 = "while w; assign a; Select w such that Follows(w, a)",
		str27 = "assign a; if ifs; variable v; Select a such that Modifies(a, v) pattern ifs(v,_,_)",
		str28 = "stmt s; Select s",
		str29 = "assign a1, a2, a3; stmt s1,s2,s3; variable v1, v2, v3; "
		"Select <a1, s1, v2> such that Uses(5, \"y\") and Follows(3, 4) pattern a1(v2, _\"x + y\"_) "
		"such that Affects(a1, a2) with a2.stmt#  = 20 such that Modifies(a3, v3) pattern a3(\"z\", _) "
		"such that Uses(s3, v1) and Modifies(s3, \"x\") and Follows(s1, s2) and Parent(3, s1) and Uses(s2, v1)",
		str30 = "Select BOOLEAN such that Affects(3, 9)",
		str31 = "while w; call calls; Select w such that Next(calls, w) and Parent(w, 9)",
		str32 = "assign a1, a2; stmtLst s; Select s such that Modifies(a1, \"x\") and Modifies(a2, \"x\") and Follows(a1, a2)",
		str33 = "assign a; while w; Select a pattern a(_, _\"haha\"_) such that Parent*(w,a)",
		str34 = "assign a1, a2; variable v; stmt s1, s2; Select s1 such that Follows(a1, a2) and Uses(a2, v) and Affects(a1, a2) with s1.stmt#  = 3 and s2.stmt# = 4",
		str35 = "call c; procedure p; variable v; Select v such that Modifies(p, v) with c.procName = p.procName",
		str36 = "assign a, a1, a2; while w; variable v, v1; if if1; "
		"Select a such that Modifies(a, v) pattern w(v, _ ) such that Follows(1,2) "
		"pattern a1 (v1, _\"x+y\"_) such that Affects (a1, a2) with a2.stmt# = 20",
		str37 = "assign a; if ifs; prog_line n; variable v; Select a "
		"such that Modifies(a, v) pattern ifs(v,_,_) with n = 01",
		str38 = "assign a; Select BOOLEAN such that Modifies(a, \"x\")",
		str39 = "constant c; Select c with c = 20";

	//cout << str29 << "\n";

	q.isValidDecAndQuery(str39);
	//cout << q.isValidExpression("x23*123-7y")<<endl; //0
	//cout << q.isValidExpression("123+-x23*123") << endl; //0
	//cout << q.isValidExpression("(x+123+y)") << endl; //1
	//cout << q.isValidExpression("x23**123") << endl; //0
	cin.ignore();
	cin.get();
	return 0;
}*/