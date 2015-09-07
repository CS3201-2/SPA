#include <string>
#include <vector>
#include <iostream>

using namespace std;
#pragma once

class QueryTree {
private:
	vector<vector<vector<string>>> qTree;
	vector<vector<string>> suchThatTree;
	vector<vector<string>> patternTree;
	vector<vector<string>> withTree;
	vector<vector<string>> variableTree;

public:
	QueryTree();

	//For inserting relationships categorized under the such that clause
	//eg. Parent, Follows, Next, Modifies, Uses
	void insertSuchThat(string rel, string arg1, string arg1_type, string arg2, string arg2_type);
	//For inserting relationships categorized under the pattern clause
	//all pattern types eg. Pattern syn(ptrn1, ptrn2)
	void insertPattern(string syn, string synType, string ptrn1, string ptrn1Type, string ptrn2, string ptrn2Type);
	//For adding variables eg. assign a, => insertVariable(a, assign)
	void insertVariable(string variable, string variableType);

	//Method not yet implemented. For accepting relationships under the "with" clause. aka insertWith

	//Getting the number of relationships that fall under the such that clause
	int getSuchThatSize();
	//Getting the number of relationships that are of Pattern types
	int getPatternSize();
	//Getting the number of variables
	int getVariableSize();

	//Obtaining the information of specific relationships that fall under the respective clauses
	//eg. To see the first relationship under the such that tree, we call getSuchThatQuery(0) and assign it to a vector
	//to read the information
	vector<string> getSuchThatQuery(int queryPos);
	vector<string> getPatternQuery(int queryPos);
	vector<string> getVariableQuery(int queryPos);
};