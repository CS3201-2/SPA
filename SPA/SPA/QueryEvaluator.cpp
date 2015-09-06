#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "QueryEvaluator.h"
#include "QueryTree.h"
#include <string>
#include <list>

using namespace::std;

//QueryEvaluator assumes items in the QueryTree are valid
//QueryEvaluator assumes invalid queries will in forwarded by QueryValidator so that error message can be printed in QueryResultProjector
//QueryEvaluator assumes each query has one such that and pattern clause (For iteration 1 and will be updated later)
//QueryEvaluator assumes 1st element in suchThat tree and 1st element in pattern tree form the first query


QueryEvaluator::QueryEvaluator() {
}


//Retrieve information from respective trees
vector<string> QueryEvaluator::getSuchThatClause(int index) {
	vector<string> tempVector;
	tempVector = QueryTree::getSuchThatQuery(index);
	return tempVector;
}

vector<string> QueryEvaluator::getPatternClause(int index) {
	vector<string> tempVector;
	tempVector = QueryTree::getPatternQuery(index);
	return tempVector;
}

vector<string> QueryEvaluator::getVarDeclaration(int index) {
	vector<string> tempVector;
	tempVector = QueryTree::getVariableQuery(index);
	return tempVector;
}


//Process Clause
string QueryEvaluator::processSuchThatClause(vector<string> tempString) {
	//Process Modifies clause
	string relationship = tempString.at(0);
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	
	if (relationship.compare("Modifies") == 0) { //Need to confirm whether Modifies is written in this format
		//General algorithm is that this function will call the modifies table and check for the respective data,
		//assuming Modifies table has all the permutations of what modifies what

		//If true, return first or second argument of Modifies depending on which is selected, else return NULL
		string result;
		return result;
	}
	else if (relationship.compare("Uses") == 0) {
		//Similar to Modifies, this function will check the Uses table with respect to data in the clause
		//If true, return first or second argument of Modifies depending on which is selected, else return NULL
		string result;
		return result;
	}
	else if (relationship.compare("Follows") == 0) {
		//This function assumes that there is a follows table containing all the permutations of follows:
		//Follows.cpp should create a table of all the possible follows relationship which is true.
		//Algorithm of Follows should be to find all children of nodes with :stmtLst and create a table containing
		//all children nodes :stmtLst with the same nesting level
		string result;
		return result;
	}
}

string QueryEvaluator::processPatternClause(vector<string> tempString) {
	string varValue = tempString.at(0);
	string varType = tempString.at(1);
	string ptrn1 = tempString.at(2);
	string ptrn1Type = tempString.at(3);
	string ptrn2 = tempString.at(4);
	string ptrn2Type = tempString.at(5);

	//Right hand side of the pattern clause is given to patternMatchAssign() in the PKB
	//patternMatchAssign() will return all assignment statements that matches the given string

	//Case 1: LHS is a constant variable
	/*
	1. We use the value of the assign synonym to search the Modifies table to get the variable that is modified
	2. If variable matches LHS, assignment statement number will be accepted 
	3. Result will be intersected with current values of the synonym
	*/
	
	//Case 2: LHS is a variable synonym or "_"
	// If the LHS is "_", no intersection is required.

	//Process while pattern node
	//Case 1: LHS is a constant variable
	/*
	1. Use a patternMatchWhile() where when given the control variable, it will return all while statements that use it.
	2. Return while statements are then used in a set intersection with respective synonyms 
	*/

	//Case 2: LHS is a variable synonym
	/*
	1. We get the control variable for each of the while statements
	2. Control variables are then intersected with the respective synonyms
	*/
}

vector<string> QueryEvaluator::combineResult(vector<string> suchThatResult, vector<string> patternResult) {
	// This function does the intersection of results from such that and pattern
	return vector<string>();
}

vector<string> QueryEvaluator::updateResult(vector<string> tempResult) {
	result.push_back(tempResult);
}

vector<vector<string>> QueryEvaluator::getResult() {
	return result;
}