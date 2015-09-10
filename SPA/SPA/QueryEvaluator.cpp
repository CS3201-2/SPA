#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "QueryEvaluator.h"
#include "QueryTree.h"
#include "ResultTable.h"
#include "PKB.h"
#include <string>
#include <list>
#include <algorithm>

using namespace::std;

//QueryEvaluator assumes items in the QueryTree are valid
//QueryEvaluator assumes invalid queries will in forwarded by QueryValidator so that error message can be printed in QueryResultProjector
//QueryEvaluator assumes each query has one such that and pattern clause (For iteration 1 and will be updated later)
//QueryEvaluator assumes 1st element in suchThat tree and 1st element in pattern tree form the first query


QueryEvaluator::QueryEvaluator(PKB my_pkb, QueryTree qt) {
	pkb = my_pkb;
	queryTree = qt;
	//isFirstClause = true;
}
// entry function for controller;
void QueryEvaluator::evaluate() {
	// first get selecet query, for iteration 1, only select first clause. hard code here
	vector<string> select = getSelectClause(0);
	int index;
	for (index = 0; index < queryTree.getSuchThatSize(); index++) {
		processSuchThatClause(getSuchThatClause(index),select);
	}
	for (index = 0; index < queryTree.getPatternSize(); index++) {
		processPatternClause(getPatternClause(index));
	}
}

//Retrieve information from respective trees
vector<string> QueryEvaluator::getSelectClause(int index) {
	vector<string> tempVector;
	tempVector = queryTree.getSelectQuery(index);
	return tempVector;
}

vector<string> QueryEvaluator::getSuchThatClause(int index) {
	vector<string> tempVector;
	tempVector = queryTree.getSuchThatQuery(index);
	return tempVector;
}

vector<string> QueryEvaluator::getPatternClause(int index) {
	vector<string> tempVector;
	tempVector = queryTree.getPatternQuery(index);
	return tempVector;
}

vector<string> QueryEvaluator::getVarDeclaration(int index) {
	vector<string> tempVector;
	tempVector = queryTree.getVariableQuery(index);
	return tempVector;
}


//Process Clause
void QueryEvaluator::processSuchThatClause(vector<string> tempString, vector<string> selectClause) {
	string relationship = tempString.at(0);

	if (relationship == "modifies") {
		resultList.push_back(processModifies(tempString, selectClause));
	}
	else if (relationship == "uses") {
		processUses(tempString, selectClause);
	}
	else if (relationship == "parent") {
		processParent(tempString, selectClause);
	}
	else if (relationship == "follows") {
		processFollows(tempString, selectClause);
	}
	else {

	}
}

bool QueryEvaluator::isInList(list<int> list, int number) {
	if (find(list.begin(), list.end(), number) != list.end()) {
		return true;
	}
	return false;
}

ResultTable QueryEvaluator::processModifies(vector<string> tempString, vector<string> selectClause) {
	string select = selectClause.at(0);
	string selectType = selectClause.at(1);

	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);

	if (arg2Type == "string") {
		int arg2ID = pkb.getVarTable().get_ID(arg2);
		list<int> modifiesLine = pkb.getModifies().get_modifies_line(arg2ID);
		if (arg1Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (find(modifiesLine.begin(), modifiesLine.end(), arg1) != modifiesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "while" || arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1);
			list<int> whileList = pkb.getWhileList();
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
			if (arg1Type == "while") {
				for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
					if (isInList(whileList, *i)) {
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else {
				for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
					if (isInList(assignList, *i)) {
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
	}

	else if( arg2Type == "variable" ) {
		Modifies modifies = pkb.getModifies();
		if (arg1Type == "prog_line") {
			list<int> varList = modifies.getModifiesVar(stoi(arg1));
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(stoi(arg1));
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg1Type == "while" || arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> whileList = pkb.getWhileList();
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
			if (arg1Type == "while") {
				for (list<int>::iterator i = whileList.begin(); i != whileList.end(); i++) {
					list<int> varList = modifies.getModifiesVar(*i);
					for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else {
				for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
					list<int> varList = modifies.getModifiesVar(*i);
					for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
	}
	else {
		return ResultTable();
	}
	
}

ResultTable QueryEvaluator::processUses(vector<string> tempString, vector<string> selectClause) {
	string select = selectClause.at(0);
	string selectType = selectClause.at(1);

	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);

	if (arg2Type == "string") {
		int arg2ID = pkb.getVarTable().get_ID(arg2);
		list<int> usesLine = pkb.getUses().get_uses_stmt(arg2ID);
		if (arg1Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (find(usesLine.begin(), usesLine.end(), arg1) != usesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "while" || arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1);
			list<int> whileList = pkb.getWhileList();
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
			if (arg1Type == "while") {
				for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
					if (isInList(whileList, *i)) {
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else {
				for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
					if (isInList(assignList, *i)) {
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
	}

	else if (arg2Type == "variable") {
		Uses uses = pkb.getUses();
		if (arg1Type == "prog_line") {
			list<int> varList = uses.getUsesVar(stoi(arg1));
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(stoi(arg1));
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg1Type == "while" || arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> whileList = pkb.getWhileList();
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
			if (arg1Type == "while") {
				for (list<int>::iterator i = whileList.begin(); i != whileList.end(); i++) {
					list<int> varList = uses.getUsesVar(*i);
					for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else {
				for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
					list<int> varList = uses.getUsesVar(*i);
					for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
	}
	else {
		return ResultTable();
	}
}

ResultTable QueryEvaluator::processParent(vector<string> tempString, vector<string> selectClause) {
	string select = selectClause.at(0);
	string selectType = selectClause.at(1);
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
}

void QueryEvaluator::processFollows(vector<string> tempString, vector<string> selectClause) {
	string select = selectClause.at(0);
	string selectType = selectClause.at(1);
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
}

	/*
	if (relationship.compare("Modifies") == 0) { 

		// Case 1: 1st argument is assignment or container statements "if", "while" where 1st arg datatype is int
		if (arg1Type.compare("int") == 0) {
			int argument1 = stoi(arg1);
			// This assumes Modifies Table is in this format: Key: var, Value: List of line numbers
			int arg2ID = pkb.getVarTable().get_ID(arg2);
			modifiesLine = pkb.getModifies().get_modifies_line(arg2ID);
			
			for (list<int>::iterator it = modifiesLine.begin(); it != modifiesLine.end(); ++it) {
				if (*it == argument1) {
				
				}
			}
		}
		// Case 2: 1st Argument is a procedure name
		/*
		else if (arg1Type.compare("Procedure") == 0) {
		}
		*/
	/*}
	else if (relationship.compare("Uses") == 0) {
		//Similar to Modifies, this function will check the Uses table with respect to data in the clause
		if (arg1Type.compare("int") == 0) {
			int argument1 = stoi(arg1);
			int arg2ID = pkb.getVarTable().get_ID(arg2);
			usesLine = pkb.getUses().get_uses_stmt(arg2ID);

			for (list<int>::iterator it = usesLine.begin(); it != usesLine.end(); ++it) {
				if (*it == argument1) {

				}
			}
		}

		// Case 2: 1st Argument is a procedure name
		/*
		else if (arg1Type.compare("Procedure") == 0) {
		}
		*/
	
/*	}
	else if (relationship.compare("Follows") == 0 || relationship.compare("Follows*") == 0) {
		//This function assumes that there is a follows table containing all the permutations of follows:
		//Follows.cpp should create a table of all the possible follows relationship which is true.
		//Algorithm of Follows should be to find all children of nodes with :stmtLst and create a table containing
		//all children nodes :stmtLst with the same nesting level
		if (relationship.compare("Follows") == 0) {
			if (arg1Type.compare("int") == 0 && arg2Type.compare("int") == 0) {
				int argument1 = stoi(arg1);
				int argument2 = stoi(arg2);
				followsList = pkb.getFollows().getFollowsStmt(argument1);

				list<int>::iterator it = followsList.begin();
				if (*it == argument2) {
					
				}
				else {
					
				}
				
			}
		}
		else {
			if (arg1Type.compare("int") == 0 && arg2Type.compare("int") == 0) {
				int argument1 = stoi(arg1);
				int argument2 = stoi(arg2);
				followsList = pkb.getFollows().getFollowsStmt(argument1);

				for (list<int>::iterator it = followsList.begin(); it != followsList.begin(); ++it) {
					if (*it == argument2) {
					
					}
				}
			}
		}
	}
}
*/
void QueryEvaluator::processPatternClause(vector<string> tempString) {
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