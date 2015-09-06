#ifndef QueryEvaluator_H
#define QueryEvaluator_H

#include <string>
#include <vector>
#include <iostream>

using namespace std; 

class QueryEvaluator {
private:

	vector<string> getSuchThatClause(int index);
	vector<string> getPatternClause(int index);
	vector<string> getVarDeclaration(int index);
	vector<string> combineResult(vector<string> suchThatResult, vector<string> patternResult);
	string processSuchThatClause(vector<string> tempString);
	string processPatternClause(vector<string> tempString);
	vector<string> updateResult(vector<string> tempResult);
	vector<vector<string>> result;
	int suchThatTreeIndex = 0;
	int patternTreeIndex = 0;

public:
	QueryEvaluator();
	vector<vector<string>> getResult();
};

#endif