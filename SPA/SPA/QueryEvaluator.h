#ifndef QueryEvaluator_H
#define QueryEvaluator_H

#include <string>
#include <vector>
#include <iostream>
#include "PKB.h"
#include "ASTNode.h"
#include "QueryTree.h"

using namespace std; 

class QueryEvaluator {
private:
	PKB pkb;
	QueryTree queryTree;
	list<ResultTable> resultList;
	//int suchThatTreeIndex = 0;
	//int patternTreeIndex = 0;
	list<ASTNode> tempResult;
	bool isInList(list<int>, int);
	bool isFirstClause;

public:
	ResultTable processModifies(vector<string> tempString, vector<string> selectClause);
	ResultTable processUses(vector<string> tempString, vector<string> selectClause);
	ResultTable processParent(vector<string> tempString, vector<string> selectClause);
	ResultTable processFollows(vector<string> tempString, vector<string> selectClause);
	QueryEvaluator( PKB, QueryTree );
	void evaluate();
	vector<vector<string>> getResult();
	vector<string> getSelectClause(int index);
	vector<string> getSuchThatClause(int index);
	vector<string> getPatternClause(int index);
	vector<string> getVarDeclaration(int index);
	vector<string> combineResult(vector<string> suchThatResult, vector<string> patternResult);
	void processSuchThatClause(vector<string> tempString, vector<string> select);
	void processPatternClause(vector<string> tempString);
	vector<string> updateResult(vector<string> tempResult);
};

#endif
