#include <string>
#include <vector>
#include <iostream>
#include "Clause.h"

using namespace std;

#ifndef QueryTree_H
#define QueryTree_H

class QueryTree {
private:
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

	int getNumOfVar(vector<string>);

public:
	QueryTree();

	//eg. Parent, Follows, Next, Modifies, Uses
	void insertSuchThat(string, vector<string>, vector<string>);
	//For inserting relationships categorized under the pattern clause
	//all pattern types eg. Pattern syn(ptrn1, ptrn2)
	void insertPattern(string, string, vector<string>, vector<string>);
	//For adding variables eg. assign a, => insertVariable(a, assign)
	void insertVariable(string, string);
	//For adding variables under the select category
	void insertSelect(vector<string>, vector<string>);
	void insertSelect(string, string);
	//For adding variables under the with category
	void insertWith(string, vector<string>, vector<string>);

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
#endif
