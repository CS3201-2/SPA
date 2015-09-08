#ifndef QueryEvaluator_H
#define QueryEvaluator_H

#include <string>
#include <vector>
#include <iostream>
#include "PKB.h"

using namespace std; 

class QueryEvaluator {
private:
	PKB pkb;
	vector<vector<string>> result;
	int suchThatTreeIndex = 0;
	int patternTreeIndex = 0;

public:
	QueryEvaluator( PKB );
	vector<vector<string>> getResult();
	vector<string> getSuchThatClause(int index, QueryTree tree);
	vector<string> getPatternClause(int index, QueryTree tree);
	vector<string> getVarDeclaration(int index, QueryTree tree);
	vector<string> combineResult(vector<string> suchThatResult, vector<string> patternResult);
	string processSuchThatClause(vector<string> tempString);
	string processPatternClause(vector<string> tempString);
	vector<string> updateResult(vector<string> tempResult);
};

#endif
