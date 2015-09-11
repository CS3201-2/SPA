#ifndef QueryEvaluator_H
#define QueryEvaluator_H

#include <string>
#include <vector>
#include <iostream>
#include "PKB.h"
#include "ASTNode.h"
#include "QueryTree.h"
#include "ResultTable.h"

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
	ResultTable processModifies(vector<string> tempString);
	ResultTable processUses(vector<string> tempString);
	ResultTable processParent(vector<string> tempString);
	ResultTable processFollows(vector<string> tempString);
	QueryEvaluator( PKB, QueryTree );
	QueryEvaluator();
	void evaluate();
	
	vector<string> getSelectClause(int index);
	vector<string> getSuchThatClause(int index);
	vector<string> getPatternClause(int index);
	vector<string> getVarDeclaration(int index);
	vector<string> combineResult(vector<string> suchThatResult, vector<string> patternResult);
	void processSuchThatClause(vector<string> tempString);
	void processPatternClause(vector<string> tempString);
	void processSelectClause(vector<string> tempString);
	//vector<string> updateResult(vector<string> tempResult);
};

#endif
