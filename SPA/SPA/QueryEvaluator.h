

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
	vector<ResultTable> selectResultList;
	bool isInList(list<int>, int);
	bool isResultEmpty(ResultTable tempResult);
	list<int> getList(string arr, string arrType);

	//split the result 
	void _updateMidResult(ResultTable newResult);
	// update the result
	void updateMidResult(ResultTable newResult);

	

	ResultTable processModifies(Clause tempString, bool useful);
	ResultTable processUses(Clause tempString, bool useful);
	ResultTable processParent(Clause tempString, bool useful);
	ResultTable processFollows(Clause tempString, bool useful);
	ResultTable processParentStar(Clause tempString, bool useful);
	ResultTable processFollowsStar(Clause tempString, bool useful);
	ResultTable processCalls(Clause tempString, bool useful);
	ResultTable processCallsStar(Clause tempString, bool useful);
	ResultTable processNext(Clause tempString, bool useful);
	ResultTable processNextStar(Clause tempString, bool useful);
	ResultTable processAffects(Clause tempString, bool useful);
	ResultTable processAffectsStar(Clause tempString, bool useful);

	bool processClause(Clause tempString, bool isUseful, bool noVar);

	bool processSuchThatClause(Clause tempString, bool useful);
	bool processSuchThatConstClause(Clause tempString, bool useful);
	bool processPatternClause(Clause tempString, bool useful);
	bool processWithClause(Clause tempString, bool useful);
	bool processWithConstClause(Clause tempString, bool useful);
	bool processSelectClause(Clause tempString, bool useful);

	ResultTable processNumberWith(Clause tempString, bool useful);
	ResultTable processNameWith(Clause tempString, bool useful);

	ResultTable processAssignPattern(Clause tempString, bool useful);
	ResultTable processWhilePattern(Clause tempString, bool useful);
	ResultTable processIfPattern(Clause tempString, bool useful);
};

#endif
