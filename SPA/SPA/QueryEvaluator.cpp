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

QueryEvaluator::QueryEvaluator() {
	pkb = PKB();
	queryTree = QueryTree();
	//isFirstClause = true;
}


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
		processSuchThatClause(getSuchThatClause(index));
	}
	for (index = 0; index < queryTree.getPatternSize(); index++) {
		processPatternClause(getPatternClause(index));
	}
	for (index = 0; index < queryTree.getSelectSize(); index++) {
		processSelectClause(getSelectClause(index));
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
void QueryEvaluator::processSuchThatClause(vector<string> tempString) {
	string relationship = tempString.at(0);

	if (relationship == "modifies") {
		resultList.push_back(processModifies(tempString));
	}
	else if (relationship == "uses") {
		resultList.push_back(processUses(tempString));
	}
	else if (relationship == "parent") {
		resultList.push_back(processParent(tempString));
	}
	else if (relationship == "follows") {
		resultList.push_back(processFollows(tempString));
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

ResultTable QueryEvaluator::processModifies(vector<string> tempString) {

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

ResultTable QueryEvaluator::processUses(vector<string> tempString) {

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

ResultTable QueryEvaluator::processParent(vector<string> tempString) {
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	AST ast = pkb.getAST();
	if (arg2Type == "prog_line") {
		
		if (arg1Type == "prog_line") {//means result table should reaturn 1 or 0;
			ResultTable tempResult = ResultTable();
			if (ast.getParent(stoi(arg2))==stoi(arg1)) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "while") {
			ResultTable tempResult = ResultTable(arg1);
			int parent = ast.getParent(stoi(arg2));
			vector<int> temp;

			if (parent != -1) {
				temp.push_back(parent);
				tempResult.addTuple(temp);
				temp.clear();
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
	}
	else if (arg2Type == "while" || arg2Type == "assign") {
		list<int> whileList = pkb.getWhileList();
		list<int> assignList = pkb.getAssignList();
		if (arg1Type == "prog_line") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			if (find(whileList.begin(), whileList.end(), stoi(arg1)) == whileList.end()) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			list<int> childList = ast.getChild(stoi(arg1));
			if (arg2Type=="while") {
				for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
					if (isInList(whileList, *t)) {
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else if(arg2Type =="assign") {
				for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
					if (isInList(assignList, *t)) {
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg1Type == "while") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1,arg2);
			if (arg2Type == "while") {
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					list<int> childList = ast.getChild(*t);
					for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
						if (isInList(whileList, *i)) {
							temp.push_back(*t);
							temp.push_back(*i);
							tempResult.addTuple(temp);
							temp.clear();
						}
					}
				}
			}
			else if (arg2Type == "assign") {
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					list<int> childList = ast.getChild(*t);
					for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
						if (isInList(assignList, *i)) {
							temp.push_back(*t);
							temp.push_back(*i);
							tempResult.addTuple(temp);
							temp.clear();
						}
					}
				}
			}
			return tempResult;
		}
		else {
			return ResultTable();
		}
	}
}

ResultTable QueryEvaluator::processFollows(vector<string> tempString) {
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	AST ast = pkb.getAST();
	if (arg2Type == "prog_line") {
		list<int> whileList = pkb.getWhileList();
		list<int> assignList = pkb.getAssignList();
		if (arg1Type == "prog_line") {//means result table should reaturn 1 or 0;
			ResultTable tempResult = ResultTable();
			if (ast.getFollowBefore(stoi(arg2)) == stoi(arg1)) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "while" || arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1);
			int brother = ast.getFollowBefore(stoi(arg2));
			if (brother == -1) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			vector<int> temp;
			if (arg1Type == "while") {
				if (isInList(whileList, brother)) {
					temp.push_back(brother);
					tempResult.addTuple(temp);
					temp.clear();
				}	
			}
			else if (arg1Type == "assign") {
				if (isInList(assignList, brother)) {
					temp.push_back(brother);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
	}
	else if (arg2Type == "while" || arg2Type == "assign") {
		list<int> whileList = pkb.getWhileList();
		list<int> assignList = pkb.getAssignList();
		if (arg1Type == "prog_line") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			int rightSibling = ast.getFollowAfter(stoi(arg1));
			if ( rightSibling == -1) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			if (arg2Type == "while") {
				if (isInList(whileList, rightSibling)) {
					temp.push_back(rightSibling);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			else if (arg2Type == "assign") {
				if (isInList(assignList, rightSibling)) {
					temp.push_back(rightSibling);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg1Type == "while" ) {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg2Type == "while") {
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					if (rightSibling != -1 && isInList(whileList, rightSibling)) {
						temp.push_back(*t);
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else if (arg2Type == "assign") {
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					if (rightSibling != -1 && isInList(assignList, rightSibling)) {
						temp.push_back(*t);
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg1Type == "assign") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg2Type == "while") {
				for (list<int>::iterator t = assignList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					if (rightSibling != -1 && isInList(whileList, rightSibling)) {
						temp.push_back(*t);
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else if (arg2Type == "assign") {
				for (list<int>::iterator t = assignList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					if (rightSibling != -1 && isInList(assignList, rightSibling)) {
						temp.push_back(*t);
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else {
			return ResultTable();
		}
	}
}

void QueryEvaluator::processPatternClause(vector<string> tempString) {
	string syn = tempString.at(0);
	string synType = tempString.at(1);
	string arg1 = tempString.at(2);
	string arg1Type = tempString.at(3);
	string arg2 = tempString.at(4);
	string arg2Type = tempString.at(5);

	AST ast = pkb.getAST();
	//syn has to be assign in prototype
	if (arg1Type == "string") {
		ResultTable tempResult = ResultTable();
		int arg1ID = pkb.getVarTable().get_ID(arg1);
		list<int> stmtList = pkb.getModifies().get_modifies_line(arg1ID);
		for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
			if (ast.matchExpression(*i,arg2)) {
				tempResult.isWholeTrue = 1;
				resultList.push_back(tempResult);
				return;
			}
		}
		tempResult.isWholeTrue = 0;
		resultList.push_back(tempResult);
		return;
	}
	else if (arg1Type == "all") {
		ResultTable tempResult = ResultTable();
		list<int> assignList = pkb.getAssignList();
		for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
			if (ast.matchExpression(*i, arg2)) {
				tempResult.isWholeTrue = 1;
				resultList.push_back(tempResult);
				return;
			}
		}
		tempResult.isWholeTrue = 0;
		resultList.push_back(tempResult);
		return;
	}
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(arg1);
		list<int> assignList = pkb.getAssignList();
		vector<int> temp;
		for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
			if (ast.matchExpression(*i, arg2)) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		resultList.push_back(tempResult);
		return;
	}
	else {
		return;
	}
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

void QueryEvaluator::processSelectClause(vector<string> tempString) {
	string syn = tempString.at(0);
	string synType = tempString.at(1);
	list<int> whileList = pkb.getWhileList();
	list<int> assignList = pkb.getAssignList();
	if (synType == "while") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		for (list<int>::iterator i = whileList.begin(); i != whileList.end; i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		resultList.push_back(tempResult);
		return;
	}
	else if (synType == "assign") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		for (list<int>::iterator i = assignList.begin(); i != assignList.end; i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		resultList.push_back(tempResult);
		return;
	}
	else if (synType == "string" || "prog_line") {
		ResultTable tempResult = ResultTable();
		tempResult.isWholeTrue = 1;
		resultList.push_back(tempResult);
		return;
	}
	else {
		return;
	}
}
vector<string> QueryEvaluator::combineResult(vector<string> suchThatResult, vector<string> patternResult) {
	// This function does the intersection of results from such that and pattern
	return vector<string>();
}
