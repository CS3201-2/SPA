//#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "QueryEvaluator.h"
#include "QueryTree.h"
#include "ResultTable.h"
#include "QueryResultProjector.h"
#include "PKB.h"
#include "SPALog.h"
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
}


QueryEvaluator::QueryEvaluator(PKB my_pkb, QueryTree qt) {
	pkb = my_pkb;
	queryTree = qt;
}
// entry function for controller;
list<string> QueryEvaluator::evaluate() {
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
	
	cout << "splited table" << endl;
	for (auto& x : resultList) {
		for (int i = 0; i < x.result.size(); ++i) {
			for (int j = 0; j < x.result[i].size(); ++j) {
				cout << x.result[i][j] << ", ";
			}
			cout << endl;
		}
		cout << "-----------------------------------" << endl;
	}

	cout << "result" << endl;
	
	QueryResultProjector qrp = QueryResultProjector(resultList, select.at(0), select.at(1), pkb);
	//cout << qrp.getResult() << endl;
	return qrp.getResult();
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
	else if (relationship == "parent*") {
		resultList.push_back(processParentStar(tempString));
	}
	else if (relationship == "follows*") {
		resultList.push_back(processFollowsStar(tempString));
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


list<int> QueryEvaluator::getList(string listName) {
	if (listName == "assign") {
		return pkb.getAssignList();
	}
	else if (listName == "while") {
		return pkb.getWhileList();
	}
	else if (listName == "if") {
		return pkb.getIfList();
	}
	else if (listName == "call") {
		return pkb.getCallList();
	}
	else if (listName == "stmt") {
		return pkb.getStmtList();
	}
	else if (listName == "procedure") {
		return pkb.getProcList();
	}
	else {
		list<int> emptyList;
		SPALog::log("Wrong list type!");
		return emptyList;
	}
}

ResultTable QueryEvaluator::processModifies(vector<string> tempString) {

	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	
	if (arg2Type == "string") {

		int arg2ID = pkb.getVarID(arg2);
		
		if (!arg2ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("arg2 is not a valid variable!\n");
			return tempResult;
		}

		list<int> modifiesLine = pkb.getModifiesSecond(arg2ID);

		if ( arg1Type == "prog_line" ) {
			ResultTable tempResult = ResultTable();

			if (!pkb.isValidStmtNo(stoi(arg1))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			if (pkb.isModifiesValid(stoi(arg1), stoi(arg2))) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if ( arg1Type == "proc_name" ) {
			ResultTable tempResult = ResultTable();
			int arg1ID = pkb.getProcID(arg1);
			if (!arg1ID) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid proc_name!\n");
				return tempResult;
			}
			
			if (find(modifiesLine.begin(), modifiesLine.end(), arg1ID) != modifiesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			// arg2 is procedure, stmt, while, assign, if, call
			list<int> targetList = getList(arg1Type);
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (isInList(targetList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
			
	}

	else if( arg2Type == "variable" ) {
		
		if (arg1Type == "prog_line") {
			if (!pkb.isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			list<int> varList = pkb.getModifiesSecond(stoi(arg1));
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg1Type == "proc_name") {
			int arg1ID = pkb.getProcID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is invalid prog_name!\n");
				return tempResult;
			}
			list<int> varList = pkb.getModifiesSecond(arg1ID);
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else {
			//procedue, while, assign, if, call, stmt
			list<int> targetList = getList(arg1);
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> varList = pkb.getModifiesSecond(*i);
				for (list<int>::iterator j = varList.begin(); j != varList.end(); j++) {
					temp.push_back(*i);
					temp.push_back(*j);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
	}
	
	else if (arg2Type == "all") {
		// if arg2Type == "all" not implement yet
	}

	else {
		SPALog::log("Error: Modifies arg2 wrong type");
	}
	
}


ResultTable QueryEvaluator::processUses(vector<string> tempString) {

	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	
	if (arg2Type == "string") {

		int arg2ID = pkb.getVarID(arg2);

		if (!arg2ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("arg2 is not a valid variable!\n");
			return tempResult;
		}

		list<int> usesLine = pkb.getUsesSecond(arg2ID);

		if (arg1Type == "prog_line") {
			ResultTable tempResult = ResultTable();

			if (!pkb.isValidStmtNo(stoi(arg1))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			if (pkb.isUsesValid(stoi(arg1), stoi(arg2))) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "proc_name") {
			ResultTable tempResult = ResultTable();
			int arg1ID = pkb.getProcID(arg1);
			if (!arg1ID) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid proc_name!\n");
				return tempResult;
			}

			if (find(usesLine.begin(), usesLine.end(), arg1ID) != usesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			// arg2 is procedure, stmt, while, assign, if, call
			list<int> targetList = getList(arg1Type);
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
				if (isInList(targetList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}

	}

	else if (arg2Type == "variable") {
		if (arg1Type == "prog_line") {
			if (!pkb.isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			list<int> varList = pkb.getUsesSecond(stoi(arg1));
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg1Type == "proc_name") {
			int arg1ID = pkb.getProcID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is invalid prog_name!\n");
				return tempResult;
			}
			list<int> varList = pkb.getUsesSecond(arg1ID);
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else {

			list<int> targetList = getList(arg1);
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> varList = pkb.getUsesSecond(*i);
				for (list<int>::iterator j = varList.begin(); j != varList.end(); j++) {
					temp.push_back(*i);
					temp.push_back(*j);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
	}

	else if (arg2Type == "all") {
		// if arg2Type == "all" not implement yet
	}

	else {
		SPALog::log("Error: Uses arg2 wrong type");
	}

}


ResultTable QueryEvaluator::processParent(vector<string> tempString) {
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	//need API for parent List
	list<int> whileList = pkb.getWhileList();
	list<int> ifList = pkb.getIfList();
	list<int> parentList = whileList;
	parentList.insert(parentList.end(),ifList.begin(),ifList.end());
	if (arg1Type == "prog_line") {
		bool isNotParrent = find(parentList.begin(), parentList.end(), stoi(arg1)) == parentList.end();
		if (!pkb.isValidStmtNo(stoi(arg1)) || isNotParrent ) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Parent arg1 is not a valid prog_line or is not a parent stmt");
			return tempResult;
		}

		list<int> childList = pkb.getParentSecond(stoi(arg1));

		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent arg2 is not a valid prog_line");
				return tempResult;
			}
			if (pkb.isParentValid(stoi(arg1), stoi(arg2))) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			//stmt, assign, while, call, if
			vector<int> temp;
			list<int> targetList = getList(arg2Type);
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
	}
	else {
		// arg1Type can be while, if, stmt
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1);
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent arg2 is not a valid prog_line");
				return tempResult;
			}
			int parent = pkb.getParentFirst(stoi(arg2));
			vector<int> temp;
			if (parent != 0 && isInList(arg1List, parent)) {
				temp.push_back(parent);
				tempResult.addTuple(temp);
				temp.clear();
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			//arg2Type can be assign, while, if, call, stmt
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1,arg2);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = pkb.getParentSecond(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(arg2List, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
	}
}

ResultTable QueryEvaluator::processFollows(vector<string> tempString) {
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	
	if (arg1Type == "prog_line") {
		if (!pkb.isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Follows arg1 is not a valid prog_line");
			return tempResult;
		}

		int littleBrother = pkb.getFollowsSecond(stoi(arg1));
		if (littleBrother == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			return tempResult;
		}
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows arg2 is not a valid prog_line");
				return tempResult;
			}
			if ( pkb.isFollowsValid(stoi(arg1), stoi(arg2))){
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt
			list<int> targetList = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			if (isInList(targetList, littleBrother)) {
				temp.push_back(littleBrother);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}

	}
	else {
		// arg1 can be while, assign, call, stmt, if
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1);
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows arg2 is not a valid prog_line");
				return tempResult;
			}
			int brother = pkb.getFollowsFirst(stoi(arg2));
			vector<int> temp;
			if (brother != 0 && isInList(arg1List, brother)) {
				temp.push_back(brother);
				tempResult.addTuple(temp);
				temp.clear();
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				int littleBrother = pkb.getFollowsSecond(*t);
				if (isInList(arg2List, littleBrother)) {
					temp.push_back(*t);
					temp.push_back(littleBrother);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
	}

}

ResultTable QueryEvaluator::processParentStar(vector<string> tempString) {

	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	//need API for parent List
	list<int> whileList = pkb.getWhileList();
	list<int> ifList = pkb.getIfList();
	list<int> parentList = whileList;
	parentList.insert(parentList.end(), ifList.begin(), ifList.end());
	if (arg1Type == "prog_line") {
		bool isNotParrent = find(parentList.begin(), parentList.end(), stoi(arg1)) == parentList.end();
		if (!pkb.isValidStmtNo(stoi(arg1)) || isNotParrent) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Parent* arg1 is not a valid prog_line or is not a parent stmt");
			return tempResult;
		}

		list<int> childList = pkb.getParentStarSecond(stoi(arg1));

		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (pkb.isParentStarValid(stoi(arg1), stoi(arg2))) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			//stmt, assign, while, call, if
			vector<int> temp;
			list<int> targetList = getList(arg2Type);
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
	}
	else {
		// arg1Type can be while, if, stmt
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1);
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> parents = pkb.getParentStarFirst(stoi(arg2));
			vector<int> temp;
			if (parents.size() != 0 )  {
				for (list<int>::iterator t = parents.begin(); t != parents.end(); t++) {
					if (isInList(arg1List, *t)) {
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			//arg2Type can be assign, while, if, call, stmt
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = pkb.getParentStarSecond(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(arg2List, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
	}
}

ResultTable QueryEvaluator::processFollowsStar(vector<string> tempString) {

	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);

	if (arg1Type == "prog_line") {
		if (!pkb.isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Follows* arg1 is not a valid prog_line");
			return tempResult;
		}

		list<int> littleBrothers = pkb.getFollowsStarSecond(stoi(arg1));
		if (littleBrothers.size() == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Follows* arg1 does not have little brothers");
			return tempResult;
		}
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (pkb.isFollowsStarValid(stoi(arg1), stoi(arg2))) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt
			list<int> targetList = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = littleBrothers.begin(); t != littleBrothers.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			
			return tempResult;
		}

	}
	else {
		// arg1 can be while, assign, call, stmt, if
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1);
			if (!pkb.isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> brothers = pkb.getFollowsStarFirst(stoi(arg2));
			vector<int> temp;
			if (brothers.size() != 0 ) {
				for (list<int>::iterator t = brothers.begin(); t != brothers.end(); t++) {
					if (isInList(arg1List, *t)) {
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = pkb.getFollowsStarSecond(*t);
				for (list<int>::iterator i = littleBrothers.begin(); i != littleBrothers.end(); i++) {
					if (isInList(arg2List, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
				
			}
			return tempResult;
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

	/*AST ast = pkb.getAST();
	//syn has to be assign in prototype
	if (arg1Type == "string") {
		ResultTable tempResult = ResultTable(syn);
		int arg1ID = pkb.getVarTable().getID(arg1);
		list<int> stmtList = pkb.getModifies().getModifiesLine(arg1ID);
		list<int> assignList = pkb.getAssignList();
		vector<int> temp;
		for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
			//cout << *i << " : " << ast.matchExpression(*i, arg2)<<endl;
			if ( isInList(assignList,*i) && ast.matchExpression(*i,arg2)) {
		
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				//tempResult.isWholeTrue = 1;
				//resultList.push_back(tempResult);
				//return;
			}
		}
		
		resultList.push_back(tempResult);
		return;
	}
	else if (arg1Type == "all") {
		ResultTable tempResult = ResultTable(syn);
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
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(syn, arg1);
		list<int> assignList = pkb.getAssignList();
		vector<int> temp;
		Modifies modifies = pkb.getModifies();
		for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
			list<int> varList = modifies.getModifiesVar(*i);
			for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
				if (ast.matchExpression(*i, arg2)) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
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
		for (list<int>::iterator i = whileList.begin(); i != whileList.end(); i++) {
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
		for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		resultList.push_back(tempResult);
		return;
	}
	else if (synType == "all") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		list<int> stmtList = assignList;
		//stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
		stmtList.insert(stmtList.end(), whileList.begin(), whileList.end());
		for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		resultList.push_back(tempResult);
		return;
	}
	else if (synType == "variable") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		list<int> stmtList;
		auto& varTable = pkb.getVarTable().varTable;
		for (map<string,int>::iterator i = varTable.begin(); i != varTable.end(); i++) {
			temp.push_back(i->second);
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
