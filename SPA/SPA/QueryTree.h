#include <string>
#include <vector>
#include <iostream>
#include "Clause.h"

using namespace std;
#pragma once

class QueryTree {
private:
	vector<vector<Clause>> qTree;

	vector<Clause> variableTree;
	vector<Clause> selectTree;
	vector<Clause> suchThatNoVarTree;
	vector<Clause> suchThatOneVarTree;
	vector<Clause> suchThatTwoVarTree;
	vector<Clause> withNoVarTree;
	vector<Clause> withOneVarTree;
	vector<Clause> withTwoVarTree;
	vector<Clause> patternOneVarTree;
	vector<Clause> patternTwoVarTree;

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

	//Obtaining the information of specific relationships that fall under the respective clauses
	//eg. To see the first relationship under the such that tree, we call getSuchThatQuery(0) and assign it to a vector
	//to read the information
	vector<Clause> getVariableTree();
	vector<Clause> getSelectTree();
	vector<Clause> getSuchThatNoVarTree();
	vector<Clause> getSuchThatOneVarTree();
	vector<Clause> getSuchThatTwoVarTree();
	vector<Clause> getWithNoVarTree();
	vector<Clause> getWithOneVarTree();
	vector<Clause> getWithTwoVarTree();
	vector<Clause> getPatternOneVarTree();
	vector<Clause> getPatternTwoVarTree();
};