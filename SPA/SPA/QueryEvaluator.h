

#include <string>
#include <vector>
#include <iostream>
#include "PKB.h"
#include "ASTNode.h"
#include "QueryTree.h"
#include "ResultTable.h"

using namespace std; 
#ifndef QueryEvaluator_H
#define QueryEvaluator_H
class QueryEvaluator {
private:
	PKB pkb;
	QueryTree queryTree;
	list<ResultTable> resultList;
	bool isInList(list<int>, int);

public:
	ResultTable processModifies(vector<string> tempString);
	ResultTable processUses(vector<string> tempString);
	ResultTable processParent(vector<string> tempString);
	ResultTable processFollows(vector<string> tempString);
	ResultTable processParentStar(vector<string> tempstring);
	ResultTable processFollowsStar(vector<string> tempstring);

	QueryEvaluator( PKB, QueryTree );
	QueryEvaluator();
	list<string> evaluate();
	
	vector<string> getSelectClause(int index);
	vector<string> getSuchThatClause(int index);
	vector<string> getPatternClause(int index);
	vector<string> getVarDeclaration(int index);
	void processSuchThatClause(vector<string> tempString);
	void processPatternClause(vector<string> tempString);
	void processSelectClause(vector<string> tempString);
};

#endif
