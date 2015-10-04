#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "QueryEvaluator.h"
#include "QueryTree.h"
#include "ResultTable.h"
#include "QueryResultProjector.h"
#include "pkb.h"
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
	queryTree = QueryTree();
}


QueryEvaluator::QueryEvaluator(QueryTree qt) {
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

	//logging should be removed before final submission
	string str;
	str = "splited table\n";
	for (auto& x : resultList) {
		for (int i = 0; i < x.result.size(); ++i) {
			for (int j = 0; j < x.result[i].size(); ++j) {
				str += to_string(x.result[i][j]) + ", ";
			}
			str += "\n";
		}
		str += "-----------------------------------\n";
	}
	SPALog::log(str);
	//logging should be removed before final submission ends
	
	QueryResultProjector qrp = QueryResultProjector(resultList, select.at(0), select.at(1));
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
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	string log = "Such that clause: "+relationship + "( " + arg1 + ":" + arg1Type + ", "+ arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

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
	else if (relationship == "calls") {
		resultList.push_back(processCalls(tempString));
	}
	else if (relationship == "call*") {
		resultList.push_back(processCallsStar(tempString));
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
		return PKB::getPKBInstance()->getAssignList();
	}
	else if (listName == "while") {
		return PKB::getPKBInstance()->getWhileList();
	}
	else if (listName == "if") {
		return PKB::getPKBInstance()->getIfList();
	}
	else if (listName == "call") {
		return PKB::getPKBInstance()->getCallList();
	}
	else if (listName == "stmt" || "all") {
		return PKB::getPKBInstance()->getStmtList();
	}
	else if (listName == "procedure") {
		return PKB::getPKBInstance()->getProcList();
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

		int arg2ID = PKB::getPKBInstance()->getVarID(arg2);
		
		if (!arg2ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("arg2 is not a valid variable!\n");
			return tempResult;
		}

		list<int> modifiesLine = PKB::getPKBInstance()->getModifiesFirst(arg2ID);

		if ( arg1Type == "prog_line" ) {
			ResultTable tempResult = ResultTable();

			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isModifiesValid(stoi(arg1), arg2ID)) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if ( arg1Type == "proc_name" ) {
			ResultTable tempResult = ResultTable();
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
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

	else if( arg2Type == "variable" || "all") {
		
		if (arg1Type == "prog_line") {
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			list<int> varList = PKB::getPKBInstance()->getModifiesSecond(stoi(arg1));
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
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is invalid prog_name!\n");
				return tempResult;
			}
			list<int> varList = PKB::getPKBInstance()->getModifiesSecond(arg1ID);
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
				list<int> varList = PKB::getPKBInstance()->getModifiesSecond(*i);
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

		int arg2ID = PKB::getPKBInstance()->getVarID(arg2);

		if (!arg2ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("arg2 is not a valid variable!\n");
			return tempResult;
		}

		list<int> usesLine = PKB::getPKBInstance()->getUsesFirst(arg2ID);

		if (arg1Type == "prog_line") {
			ResultTable tempResult = ResultTable();

			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isUsesValid(stoi(arg1), arg2ID)) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "proc_name") {
			ResultTable tempResult = ResultTable();
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
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

	else if (arg2Type == "variable" || "all") {
		if (arg1Type == "prog_line") {
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			list<int> varList = PKB::getPKBInstance()->getUsesSecond(stoi(arg1));
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
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				SPALog::log("arg1 is invalid prog_name!\n");
				return tempResult;
			}
			list<int> varList = PKB::getPKBInstance()->getUsesSecond(arg1ID);
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
				list<int> varList = PKB::getPKBInstance()->getUsesSecond(*i);
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

	else {
		SPALog::log("Error: Uses arg2 wrong type");
	}

}

ResultTable QueryEvaluator::processParent(vector<string> tempString) {
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	
	list<int> parentList = PKB::getPKBInstance()->getParentList();

	if (arg1Type == "prog_line") {
		bool isNotParrent = !isInList(parentList, stoi(arg1));
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1)) || isNotParrent ) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Parent arg1 is not a valid prog_line or is not a parent stmt");
			return tempResult;
		}

		list<int> childList = PKB::getPKBInstance()->getParentSecond(stoi(arg1));

		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isParentValid(stoi(arg1), stoi(arg2))) {
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
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent arg2 is not a valid prog_line");
				return tempResult;
			}
			int parent = PKB::getPKBInstance()->getParentFirst(stoi(arg2));
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
			if (arg1 == arg2) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = PKB::getPKBInstance()->getParentSecond(*t);
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
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Follows arg1 is not a valid prog_line");
			return tempResult;
		}

		int littleBrother = PKB::getPKBInstance()->getFollowsSecond(stoi(arg1));
		if (littleBrother == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			return tempResult;
		}
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows arg2 is not a valid prog_line");
				return tempResult;
			}
			if ( PKB::getPKBInstance()->isFollowsValid(stoi(arg1), stoi(arg2))){
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
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows arg2 is not a valid prog_line");
				return tempResult;
			}
			int brother = PKB::getPKBInstance()->getFollowsFirst(stoi(arg2));
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
			if (arg1 == arg2) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				int littleBrother = PKB::getPKBInstance()->getFollowsSecond(*t);
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
	
	list<int> parentList = PKB::getPKBInstance()->getParentList();
	
	if (arg1Type == "prog_line") {
		bool isNotParrent = !isInList(parentList, stoi(arg1));
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1)) || isNotParrent) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Parent* arg1 is not a valid prog_line or is not a parent stmt");
			return tempResult;
		}

		list<int> childList = PKB::getPKBInstance()->getParentStarSecond(stoi(arg1));

		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isParentStarValid(stoi(arg1), stoi(arg2))) {
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
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Parent* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> parents = PKB::getPKBInstance()->getParentStarFirst(stoi(arg2));
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
			if (arg1 == arg2) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = PKB::getPKBInstance()->getParentStarSecond(*t);
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
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Follows* arg1 is not a valid prog_line");
			return tempResult;
		}

		list<int> littleBrothers = PKB::getPKBInstance()->getFollowsStarSecond(stoi(arg1));
		if (littleBrothers.size() == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("Follows* arg1 does not have little brothers");
			return tempResult;
		}
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isFollowsStarValid(stoi(arg1), stoi(arg2))) {
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
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				SPALog::log("Follows* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> brothers = PKB::getPKBInstance()->getFollowsStarFirst(stoi(arg2));
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
			if (arg1 == arg2) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getFollowsStarSecond(*t);
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

ResultTable QueryEvaluator::processCalls(vector<string> tempString) {
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	if (arg1Type == "proc_name") {
		int arg1ID = PKB::getPKBInstance()->getProcID(arg1);

		if (!arg1ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("arg1 is not a valid proc id!\n");
			return tempResult;
		}

		list<int> procedureCalled = PKB::getPKBInstance()->getCallsSecond(arg1ID);

		if (arg2Type == "proc_name") {
			ResultTable tempResult = ResultTable();
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg2 is not a valid proc id!\n");
				return tempResult;
			}

			if (PKB::getPKBInstance()->isCallsValid(arg1ID, arg2ID)) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg2Type == "procedure" || "all") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = procedureCalled.begin(); i != procedureCalled.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else if (arg1Type == "procedure" || "all") {
		if (arg2Type == "proc_name") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg2 is not a valid proc id!\n");
				return tempResult;
			}

			list<int> procedureCalls = PKB::getPKBInstance()->getCallsFirst(arg2ID);

			for (list<int>::iterator i = procedureCalls.begin(); i != procedureCalls.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "procedure" || "all") {
			ResultTable tempResult = ResultTable(arg1,arg2);
			vector<int> temp;
			list<int> procList = PKB::getPKBInstance()->getProcList();
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsSecond(*i);
				for (list<int>::iterator t = procedureCalled.begin(); t != procedureCalled.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else {
		SPALog::log("arg1 is not valid!\n");
		return ResultTable();
	}

}

ResultTable QueryEvaluator::processCallsStar(vector<string> tempString) {
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	if (arg1Type == "proc_name") {
		int arg1ID = PKB::getPKBInstance()->getProcID(arg1);

		if (!arg1ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			SPALog::log("arg1 is not a valid proc id!\n");
			return tempResult;
		}

		list<int> procedureCalled = PKB::getPKBInstance()->getCallsStarSecond(arg1ID);

		if (arg2Type == "proc_name") {
			ResultTable tempResult = ResultTable();
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg2 is not a valid proc id!\n");
				return tempResult;
			}

			if (PKB::getPKBInstance()->isCallsStarValid(arg1ID, arg2ID)) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg2Type == "procedure" || "all") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = procedureCalled.begin(); i != procedureCalled.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else if (arg1Type == "procedure" || "all") {
		if (arg2Type == "proc_name") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.isWholeTrue = 0;
				SPALog::log("arg2 is not a valid proc id!\n");
				return tempResult;
			}

			list<int> procedureCalls = PKB::getPKBInstance()->getCallsStarFirst(arg2ID);

			for (list<int>::iterator i = procedureCalls.begin(); i != procedureCalls.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "procedure" || "all") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> procList = PKB::getPKBInstance()->getProcList();
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsStarSecond(*i);
				for (list<int>::iterator t = procedureCalled.begin(); t != procedureCalled.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else {
		SPALog::log("arg1 is not valid!\n");
		return ResultTable();
	}

}

void QueryEvaluator::processPatternClause(vector<string> tempString) {
	
	string synType = tempString.at(1);
	string arg1 = tempString.at(2);
	string arg1Type = tempString.at(3);
	string arg2 = tempString.at(4);
	string arg2Type = tempString.at(5);
	
	string log = "Pattern clause: " + synType + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	if (synType == "assign") {
		resultList.push_back(processAssignPattern(tempString));
	}
	else if (synType == "while") {
		resultList.push_back(processWhilePattern(tempString));
	}
	else if (synType == "if") {
		resultList.push_back(processIfPattern(tempString));
	}
	else {

	}
	
}

ResultTable QueryEvaluator::processAssignPattern(vector<string> tempString) {
	string syn = tempString.at(0);
	string synType = tempString.at(1);
	string arg1 = tempString.at(2);
	string arg1Type = tempString.at(3);
	string arg2 = tempString.at(4);
	string arg2Type = tempString.at(5);

	if (arg1Type == "string") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		if (arg2Type == "all") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithFirstExact(arg1);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		else {
			//iter 1 "constant or string" or variable
			string arg2Trim = arg2.substr(2, arg2.length() - 4);
			list<int> assignList = PKB::getPKBInstance()->getAssignWithBoth(arg1, arg2Trim);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		return tempResult;
	}
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(syn, arg1);
		vector<int> temp;
		if (arg2Type == "all") {
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				list<int> assignList = PKB::getPKBInstance()->getAssignWithFirstExact(PKB::getPKBInstance()->getVarName(*i));
				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
					temp.push_back(*t);
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
		}
		else {
			//iter 1 "constant or string" or "variable"
			string arg2Trim = arg2.substr(2, arg2.length() - 4);
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecond(arg2Trim);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				list<int> modifiedVarList = PKB::getPKBInstance()->getModifiesSecond(*i);
				for (list<int>::iterator t = modifiedVarList.begin(); t != modifiedVarList.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
		}
		
		return tempResult;
	}
	else if (arg1Type == "all") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		if (arg2Type == "all") {
			list<int> assignList = PKB::getPKBInstance()->getAssignList();
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		else {
			// arg2Type is constant or string or variable
			string arg2Trim = arg2.substr(2, arg2.length() - 4);
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecond(arg2Trim);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		return tempResult;
	}
	else {
		SPALog::log("pattern arg1 type wrong");
		return ResultTable();
	}
}

ResultTable QueryEvaluator::processWhilePattern(vector<string> tempString) {
	string syn = tempString.at(0);
	string synType = tempString.at(1);
	string arg1 = tempString.at(2);
	string arg1Type = tempString.at(3);
	string arg2 = tempString.at(4);
	string arg2Type = tempString.at(5);

	if (arg1Type == "string") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		int arg1ID = PKB::getPKBInstance()->getVarID(arg1);
		if (!arg1ID) {
			tempResult.isWholeTrue = 0;
			SPALog::log("arg1 is not a valid variable!\n");
			return tempResult;
		}

		list<int> whileList = PKB::getPKBInstance()->getWhileWithFirstExact(arg1);
		for (list<int>::iterator i = whileList.begin(); i != whileList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		return tempResult;
	}
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(syn, arg1);
		vector<int> temp;
		list<int> varList = PKB::getPKBInstance()->getVarList();
		for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
			string varName = PKB::getPKBInstance()->getVarName(*i);
			list<int> whileList = PKB::getPKBInstance()->getWhileWithFirstExact(varName);
			for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
				temp.push_back(*t);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		return tempResult;
	}
	else {
	 //arg1Type == all
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		
		list<int> whileList = PKB::getPKBInstance()->getWhileList();
		for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
			temp.push_back(*t);
			tempResult.addTuple(temp);
			temp.clear();
		}
		return tempResult;
	}
}

ResultTable QueryEvaluator::processIfPattern(vector<string> tempString) {
	string syn = tempString.at(0);
	string synType = tempString.at(1);
	string arg1 = tempString.at(2);
	string arg1Type = tempString.at(3);
	string arg2 = tempString.at(4);
	string arg2Type = tempString.at(5);

	if (arg1Type == "string") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		int arg1ID = PKB::getPKBInstance()->getVarID(arg1);
		if (!arg1ID) {
			tempResult.isWholeTrue = 0;
			SPALog::log("arg1 is not a valid variable!\n");
			return tempResult;
		}

		list<int> ifList = PKB::getPKBInstance()->getIfWithFirstExact(arg1);
		for (list<int>::iterator i = ifList.begin(); i != ifList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		return tempResult;
	}
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(syn, arg1);
		vector<int> temp;
		list<int> varList = PKB::getPKBInstance()->getVarList();
		for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
			string varName = PKB::getPKBInstance()->getVarName(*i);
			list<int> ifList = PKB::getPKBInstance()->getIfWithFirstExact(varName);
			for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
				temp.push_back(*t);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		return tempResult;
	}
	else {
		//arg1Type == all
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;

		list<int> ifList = PKB::getPKBInstance()->getIfList();
		for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
			temp.push_back(*t);
			tempResult.addTuple(temp);
			temp.clear();
		}
		return tempResult;
	}
}

void QueryEvaluator::processSelectClause(vector<string> tempString) {
	string syn = tempString.at(0);
	string synType = tempString.at(1);
	string log = "Select clause: select " + syn + ":" + synType +"\n";
	SPALog::log(log);

	if (synType == "variable") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		list<int> varTable = PKB::getPKBInstance()->getVarList();
		for (list<int>::iterator i = varTable.begin(); i != varTable.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		resultList.push_back(tempResult);
		return;
	}
	else if (synType == "procedure") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		list<int> procList = PKB::getPKBInstance()->getProcList();
		for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		resultList.push_back(tempResult);
		return;
	}
	else if (synType == "boolean") {
		ResultTable tempResult = ResultTable(syn);
		tempResult.isWholeTrue = 1;
		resultList.push_back(tempResult);
		return;
	}
	else {
		list<int> targetList = getList(synType);
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		resultList.push_back(tempResult);
		return;
	}
}
