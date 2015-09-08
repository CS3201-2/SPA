#ifndef QueryEvaluator_H
#define QueryEvaluator_H

#include <string>
#include <vector>
#include <iostream>
#include "PKB.h"
#include "QueryTree.h"

using namespace std; 

class QueryEvaluator {
private:
	PKB pkb;
	QueryTree queryTree;
	vector<vector<string>> result;
	int suchThatTreeIndex = 0;
	int patternTreeIndex = 0;

public:
	QueryEvaluator( PKB, QueryTree );
	void evaluate();
	vector<vector<string>> getResult();
	vector<string> getSuchThatClause(int index);
	vector<string> getPatternClause(int index);
	vector<string> getVarDeclaration(int index);
	vector<string> combineResult(vector<string> suchThatResult, vector<string> patternResult);
	void processSuchThatClause(vector<string> tempString);
	void processPatternClause(vector<string> tempString);
	vector<string> updateResult(vector<string> tempResult);
};

#endif
