

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
	//intermidiate result 
	vector<ResultTable> midResult;
	// final result pass to projector
	vector<ResultTable> resultList;
	bool isInList(list<int>, int);
	bool isResultEmpty(ResultTable tempResult);
	list<int> getList(string arr, string arrType);

	//split the result 
	void _updateMidResult(ResultTable newResult);
	// update the result
	void updateMidResult(ResultTable newResult);

	

	ResultTable processModifies(Clause tempString);
	ResultTable processUses(Clause tempString);
	ResultTable processParent(Clause tempString);
	ResultTable processFollows(Clause tempString);
	ResultTable processParentStar(Clause tempString);
	ResultTable processFollowsStar(Clause tempString);
	ResultTable processCalls(Clause tempString);
	ResultTable processCallsStar(Clause tempString);
	ResultTable processNext(Clause tempString);
	ResultTable processNextStar(Clause tempString);
	ResultTable processAffects(Clause tempString);
	ResultTable processAffectsStar(Clause tempString);

	bool processSuchThatClause(Clause tempString);
	bool processSuchThatConstClause(Clause tempString);
	bool processPatternClause(Clause tempString);
	bool processWithClause(Clause tempString);
	bool processWithConstClause(Clause tempString);
	bool processSelectClause(Clause tempString);

	ResultTable processNumberWith(Clause tempString);
	ResultTable processNameWith(Clause tempString);

	ResultTable processAssignPattern(Clause tempString);
	ResultTable processWhilePattern(Clause tempString);
	ResultTable processIfPattern(Clause tempString);
};

#endif
