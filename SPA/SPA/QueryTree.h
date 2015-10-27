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
	vector<vector<string>> selectTree;
	vector<vector<string>> suchThatConstTree;
	vector<vector<string>> withConstTree;

public:
	QueryTree();

	//For inserting relationships categorized under the such that clause
	//eg. Parent, Follows, Next, Modifies, Uses
	void insertSuchThat(string rel, vector<string> arrVar, vector<string> argType);
	//For inserting relationships categorized under the pattern clause
	//all pattern types eg. Pattern syn(ptrn1, ptrn2)
	void insertPattern(string syn, string synType, vector<string> arrPtrn, vector<string> ptrnType);
	//For adding variables eg. assign a, => insertVariable(a, assign)
	void insertVariable(string variable, string variableType);
	//For adding variables under the select category
	void insertSelect(vector<string> var, vector<string> varType);
	void insertSelect(string var, string varType);
	//For adding variables under the with category
	void insertWith(string rel, vector<string> arrWith, vector<string> withType);

	//Getting the number of relationships that fall under the such that clause
	int getSuchThatSize();
	//Getting the relationships that are under the such that clause but are constants
	int getSuchThatConstSize();
	//Getting the number of relationships that are of Pattern types
	int getPatternSize();
	//Getting the number of variables
	int getVariableSize();
	//Getting the number of variables under select
	int getSelectSize();
	//Getting the number of relationships under the with clause
	int getWithSize();
	//Getting the relationships under the with clause but are constants
	int getWithConstSize();

	//Obtaining the information of specific relationships that fall under the respective clauses
	//eg. To see the first relationship under the such that tree, we call getSuchThatQuery(0) and assign it to a vector
	//to read the information
	vector<string> getSuchThatQuery(int queryPos);
	vector<string> getSuchThatConstQuery(int queryPos);
	vector<string> getPatternQuery(int queryPos);
	vector<string> getVariableQuery(int queryPos);
	vector<string> getSelectQuery(int queryPos);
	vector<string> getWithQuery(int queryPos);
	vector<string> getWithConstQuery(int queryPos);
};