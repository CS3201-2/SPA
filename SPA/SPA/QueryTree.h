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

	int getNumOfVar(vector<string>);

public:
	QueryTree();

	void insertSuchThat(string, vector<string>, vector<string>);
	void insertPattern(string, string, vector<string>, vector<string>);
	void insertVariable(string, string);
	void insertSelect(vector<string>, vector<string>);
	void insertSelect(string, string);
	void insertWith(string, vector<string>, vector<string>);

	vector<Clause> getVariableTree();
	Clause getSelectClause();
	vector<Clause> getUsefulNoVarTree();
	vector<Clause> getUsefulOneVarTree();
	vector<Clause> getUsefulTwoVarTree();

	//group method
	void grouping();
	bool hasCommon(vector<string>, vector<string>);
};
#endif
