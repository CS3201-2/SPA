#include <string>
#include <vector>
#include <iostream>
#include "Clause.h"

using namespace std;

#ifndef QueryTree_H
#define QueryTree_H

class QueryTree {
private:
	vector<Clause> _allClauses;
	vector<Clause> _variableTree;
	Clause _selectClause;
	vector<bool> _useful;
	vector<Clause> _usefulNoVarTree;
	vector<Clause> _usefulOneVarTree;
	vector<Clause> _usefulTwoVarTree;
	vector<Clause> _uselessOneVarTree;
	vector<Clause> _uselessTwoVarTree;

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
	Clause getSelectClause();
	vector<Clause> getUsefulNoVarTree();
	vector<Clause> getUsefulOneVarTree();
	vector<Clause> getUsefulTwoVarTree();
	vector<Clause> getUselessOneVarTree();
	vector<Clause> getUselessTwoVarTree();

	//group method
	void grouping();
	bool hasCommon(vector<string>, vector<string>);
};
#endif
