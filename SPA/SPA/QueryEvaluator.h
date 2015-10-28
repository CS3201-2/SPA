

#include <string>
#include <vector>
#include <iostream>
#include "PKB.h"
#include "QueryTree.h"
#include "ResultTable.h"

using namespace std; 
#ifndef QueryEvaluator_H
#define QueryEvaluator_H
class QueryEvaluator {
public:
	QueryEvaluator(QueryTree);
	QueryEvaluator();
	list<string> evaluate();

private:
	QueryTree queryTree;
	vector<ResultTable> resultList;
	bool isInList(list<int>, int);
	bool isResultEmpty(ResultTable tempResult);
	list<int> getList(string listName);

	ResultTable processModifies(vector<string> tempString);
	ResultTable processUses(vector<string> tempString);
	ResultTable processParent(vector<string> tempString);
	ResultTable processFollows(vector<string> tempString);
	ResultTable processParentStar(vector<string> tempString);
	ResultTable processFollowsStar(vector<string> tempString);
	ResultTable processCalls(vector<string> tempString);
	ResultTable processCallsStar(vector<string> tempString);
	ResultTable processNext(vector<string> tempString);
	ResultTable processNextStar(vector<string> tempString);

	bool processSuchThatClause(Clause tempString);
	bool processSuchThatConstClause(Clause tempString);
	bool processPatternClause(Clause tempString);
	bool processWithClause(Clause tempString);
	bool processWithConstClause(Clause tempString);
	bool processSelectClause(Clause tempString);

	ResultTable processNumberWith(vector<string> tempString);
	ResultTable processNameWith(vector<string> tempString);

	ResultTable processAssignPattern(vector<string> tempString);
	ResultTable processWhilePattern(vector<string> tempString);
	ResultTable processIfPattern(vector<string> tempString);
};

#endif
