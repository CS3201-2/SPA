

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
	list<ResultTable> resultList;
	bool isInList(list<int>, int);
	list<int> getList(string listName);

	ResultTable processModifies(vector<string> tempString);
	ResultTable processUses(vector<string> tempString);
	ResultTable processParent(vector<string> tempString);
	ResultTable processFollows(vector<string> tempString);
	ResultTable processParentStar(vector<string> tempString);
	ResultTable processFollowsStar(vector<string> tempString);
	ResultTable processCalls(vector<string> tempString);
	ResultTable processCallsStar(vector<string> tempString);

	vector<string> getSelectClause(int index);
	vector<string> getSuchThatClause(int index);
	vector<string> getPatternClause(int index);
	vector<string> getVarDeclaration(int index);

	void processSuchThatClause(vector<string> tempString);
	void processPatternClause(vector<string> tempString);
	void processSelectClause(vector<string> tempString);

	ResultTable processAssignPattern(vector<string> tempString);
	ResultTable processWhilePattern(vector<string> tempString);
	ResultTable processIfPattern(vector<string> tempString);
};

#endif
