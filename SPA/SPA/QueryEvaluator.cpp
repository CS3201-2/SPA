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

bool QueryEvaluator::isValidStmt(int stmt) {
	/*if (stmt < 0 || stmt > pkb.getMaxStmt()) {
		return false;
	}*/
	return true;
}

ResultTable QueryEvaluator::processModifies(vector<string> tempString) {

	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	//cout << arg1Type << endl;
	if (arg2Type == "string") {
		int arg2ID = pkb.getVarTable().getID(arg2);
		if (!arg2ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			return tempResult;
		}
		list<int> modifiesLine = pkb.getModifies().getModifiesLine(arg2ID);
		if ( arg1Type == "prog_line" ) {
			if (!isValidStmt(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			ResultTable tempResult = ResultTable();
			if (find(modifiesLine.begin(), modifiesLine.end(), stoi(arg1)) != modifiesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if ( arg1Type == "proc_name" ) {
			int arg1ID = pkb.getProcTable().getID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			ResultTable tempResult = ResultTable();
			if (find(modifiesLine.begin(), modifiesLine.end(), arg1ID) != modifiesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if ( arg1Type == "procedure" ) {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (*i < 0) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if ( arg1Type == "while" ){
			// arg2 is stmt, while, assign, if, call
			ResultTable tempResult = ResultTable(arg1);
			list<int> whileList = pkb.getWhileList();
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (isInList(whileList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if ( arg1Type == "assign" ) {
			ResultTable tempResult = ResultTable(arg1);
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (isInList(assignList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if ( arg1Type == "call" ) {
			/*
			ResultTable tempResult = ResultTable(arg1);
			list<int> callList = pkb.getCallList();
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (isInList(callList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
			*/
		} 
		else if ( arg1Type == "if" ) {
			/*
			ResultTable tempResult = ResultTable(arg1);
			list<int> ifList = pkb.getIfList();
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (isInList(ifList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;*/
		}
		else if (arg1Type == "stmt") {
			/*
			ResultTable tempResult = ResultTable(arg1);
			list<int> stmtList = pkb.getStmtList();
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (isInList(stmtList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;*/
		}
		else {
			SPALog::log("Error: Modifies arg1 wrong type, arg2 string");
		}
			
	}

	else if( arg2Type == "variable" ) {
		Modifies modifies = pkb.getModifies();
		if (arg1Type == "prog_line") {
			if (!isValidStmt(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			list<int> varList = modifies.getModifiesVar(stoi(arg1));
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
			int arg1ID = pkb.getProcTable().getID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			list<int> varList = modifies.getModifiesVar(arg1ID);
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg1Type == "procedure") {
			/*
			int procList = pkb.getProcTable().getAllID();
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> varList = modifies.getModifiesVar(*i);
				for (list<int>::iterator j = varList.begin(); j != varList.end(); j++) {
					temp.push_back(*i);
					temp.push_back(*j);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;*/
		}
		else if (arg1Type == "while") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> whileList = pkb.getWhileList();
			vector<int> temp;
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
		else if (arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
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
		else if (arg1Type == "call") {/*
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> callList = pkb.getCallList();
			vector<int> temp;
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				list<int> varList = modifies.getModifiesVar(*i);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}*/
		}
		else if (arg1Type == "if") {/*
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> ifList = pkb.getIfList();
			vector<int> temp;
			for (list<int>::iterator i = ifList.begin(); i != ifList.end(); i++) {
				list<int> varList = modifies.getModifiesVar(*i);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}*/
		}
		else if (arg1Type == "stmt") {/*
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> stmtList = pkb.getStmtList();
			vector<int> temp;
			for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
				list<int> varList = modifies.getModifiesVar(*i);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}*/
		}
		else {
			SPALog::log("Error: Modifies arg1 wrong type, arg2 variable");
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
		int arg2ID = pkb.getVarTable().getID(arg2);
		if (!arg2ID) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			return tempResult;
		}
		list<int> usesLine = pkb.getUses().getUsesStmt(arg2ID);
		if (arg1Type == "prog_line") {
			if (!isValidStmt(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			ResultTable tempResult = ResultTable();
			if (find(usesLine.begin(), usesLine.end(), stoi(arg1)) != usesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "proc_name") {
			int arg1ID = pkb.getProcTable().getID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			ResultTable tempResult = ResultTable();
			if (find(usesLine.begin(), usesLine.end(), arg1ID) != usesLine.end()) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg1Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
				if (*i < 0) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg1Type == "while") {
			// arg2 is stmt, while, assign, if, call
			ResultTable tempResult = ResultTable(arg1);
			list<int> whileList = pkb.getWhileList();
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
				if (isInList(whileList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1);
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
				if (isInList(assignList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg1Type == "call") {
			/*
			ResultTable tempResult = ResultTable(arg1);
			list<int> callList = pkb.getCallList();
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
			if (isInList(callList, *i)) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
			}
			}
			return tempResult;
			*/
		}
		else if (arg1Type == "if") {
			/*
			ResultTable tempResult = ResultTable(arg1);
			list<int> ifList = pkb.getIfList();
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
			if (isInList(ifList, *i)) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
			}
			}
			return tempResult;*/
		}
		else if (arg1Type == "stmt") {
			/*
			ResultTable tempResult = ResultTable(arg1);
			list<int> stmtList = pkb.getStmtList();
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
			if (isInList(stmtList, *i)) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
			}
			}
			return tempResult;*/
		}
		else {
			SPALog::log("Error: Uses arg1 wrong type, arg2 type string");
		}

	}

	else if (arg2Type == "variable") {
		Uses uses = pkb.getUses();
		if (arg1Type == "prog_line") {
			if (!isValidStmt(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			list<int> varList = uses.getUsesVar(stoi(arg1));
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
			int arg1ID = pkb.getProcTable().getID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			list<int> varList = uses.getUsesVar(arg1ID);
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg1Type == "procedure") {
			/*
			int procList = pkb.getProcTable().getAllID();
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
			list<int> varList = uses.getUsesVar(*i);
			for (list<int>::iterator j = varList.begin(); j != varList.end(); j++) {
			temp.push_back(*i);
			temp.push_back(*j);
			tempResult.addTuple(temp);
			temp.clear();
			}
			}
			return tempResult;*/
		}
		else if (arg1Type == "while") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> whileList = pkb.getWhileList();
			vector<int> temp;
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
		else if (arg1Type == "assign") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			list<int> assignList = pkb.getAssignList();
			vector<int> temp;
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
		else if (arg1Type == "call") {/*
									  ResultTable tempResult = ResultTable(arg1, arg2);
									  list<int> callList = pkb.getCallList();
									  vector<int> temp;
									  for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
									  list<int> varList = uses.getUsesVar(*i);
									  for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
									  temp.push_back(*i);
									  temp.push_back(*t);
									  tempResult.addTuple(temp);
									  temp.clear();
									  }
									  }*/
		}
		else if (arg1Type == "if") {/*
									ResultTable tempResult = ResultTable(arg1, arg2);
									list<int> ifList = pkb.getIfList();
									vector<int> temp;
									for (list<int>::iterator i = ifList.begin(); i != ifList.end(); i++) {
									list<int> varList = uses.getUsesVar(*i);
									for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
									temp.push_back(*i);
									temp.push_back(*t);
									tempResult.addTuple(temp);
									temp.clear();
									}
									}*/
		}
		else if (arg1Type == "stmt") {/*
									  ResultTable tempResult = ResultTable(arg1, arg2);
									  list<int> stmtList = pkb.getStmtList();
									  vector<int> temp;
									  for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
									  list<int> varList = uses.getUsesVar(*i);
									  for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
									  temp.push_back(*i);
									  temp.push_back(*t);
									  tempResult.addTuple(temp);
									  temp.clear();
									  }
									  }*/
		}
		else {
			SPALog::log("Error: Uses arg1 wrong type, arg2 variable");
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
	list<int> whileList = pkb.getWhileList();
	list<int> assignList = pkb.getAssignList();
	//list<int> stmtList = pkb.getStmtList();
	//list<int> ifList = pkb.getIfList();
    //list<int> callList = pkb.getCallList();
	list<int> parentList = whileList;
	//parentList.insert(parentList.end(),ifList.begin(),ifList.end());
	if (arg1Type == "prog_line") {
		bool isNotParrent = find(parentList.begin(), parentList.end(), stoi(arg1)) == parentList.end();
		if (!isValidStmt(stoi(arg1)) || isNotParrent ) {
			ResultTable tempResult = ResultTable();
			tempResult.isWholeTrue = 0;
			return tempResult;
		}

		list<int> childList = pkb.getParent().getChildStmt(stoi(arg1));

		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable();
			if (!isValidStmt(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			if (pkb.getParent().getParentStmt(stoi(arg2)) == stoi(arg1)) {
				tempResult.isWholeTrue = 1;
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg2Type == "assign") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(assignList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg2Type == "while") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(whileList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg2Type == "if") {
			/*vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(ifList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;*/
		}
		else if (arg2Type == "call") {
			/*vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(callList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;*/
		}
		else if (arg2Type == "stmt") {
			//does stmt means every stmt line?
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
			}
			return tempResult;
		}
		else {
			SPALog::log("Error: Parent arg1 prog_line, arg2 wrong type");
		}

	}
	else if (arg1Type == "while") {
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1);
			if (!isValidStmt(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			int parent = pkb.getParent().getParentStmt(stoi(arg2));
			vector<int> temp;
			if (parent != 0 && isInList(whileList, parent)) {
				temp.push_back(parent);
				tempResult.addTuple(temp);
				temp.clear();
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg2Type == "assign") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1,arg2);
			for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(assignList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg2Type == "while") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			//this does not affect results but may increase the speed
			if (arg1 == arg2) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(whileList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg2Type == "if") {
			/*vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(ifList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;*/
		}
		else if (arg2Type == "call") {
			/*vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(callList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;*/
		}
		else if (arg2Type == "stmt") {
			/*vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(stmtList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;*/
		}
		else {
			SPALog::log("Error: Parent arg1 while, arg2 wrong type");
		}
	}
	else if (arg1Type == "if") {
		if (arg2Type == "prog_line") {
			/*ResultTable tempResult = ResultTable(arg1);
			if (!isValidStmt(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			int parent = pkb.getParent().getParentStmt(stoi(arg2));
			vector<int> temp;
			if (parent != 0 && isInList(ifList, parent)) {
				temp.push_back(parent);
				tempResult.addTuple(temp);
				temp.clear();
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;*/
		}
		else if (arg2Type == "assign") {
			/*
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(assignList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;*/
		}
		else if (arg2Type == "while") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			
			/*for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(whileList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}*/
			return tempResult;
		}
		else if (arg2Type == "if") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			//this does not affect results but may increase the speed
			if (arg1 == arg2) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			/*for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(ifList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;*/
		}
		else if (arg2Type == "call") {
			/*vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
			list<int> childList = pkb.getParent().getChildStmt(*t);
			for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
			if (isInList(callList, *i)) {
			temp.push_back(*t);
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
			}
			}
			}
			return tempResult;*/
		}
		else if (arg2Type == "stmt") {
			/*vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
			list<int> childList = pkb.getParent().getChildStmt(*t);
			for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
			if (isInList(stmtList, *i)) {
			temp.push_back(*t);
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
			}
			}
			}
			return tempResult;*/
		}
		else {
			SPALog::log("Error: Parent arg1 if, arg2 wrong type");
		}
	}
	else if (arg1Type == "stmt") {
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1);
			if (!isValidStmt(stoi(arg2))) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			int parent = pkb.getParent().getParentStmt(stoi(arg2));
			vector<int> temp;
			if (parent != 0 ) {
				temp.push_back(parent);
				tempResult.addTuple(temp);
				temp.clear();
			}
			else {
				tempResult.isWholeTrue = 0;
			}
			return tempResult;
		}
		else if (arg2Type == "assign") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = parentList.begin(); t != parentList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(assignList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg2Type == "while") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = parentList.begin(); t != parentList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					if (isInList(whileList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg2Type == "if") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = parentList.begin(); t != parentList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					/*if (isInList(ifList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}*/
				}
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = parentList.begin(); t != parentList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					/*if (isInList(callList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}*/
				}
			}
			return tempResult;
		}
		else if (arg2Type == "stmt") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			//this does not affect results but may increase the speed
			if (arg1 == arg2) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			for (list<int>::iterator t = parentList.begin(); t != parentList.end(); t++) {
				list<int> childList = pkb.getParent().getChildStmt(*t);
				for (list<int>::iterator i = childList.begin(); i != childList.end(); i++) {
					/*if (isInList(stmtList, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}*/
				}
			}
			return tempResult;
		}
		//this does not affect results but may increase the speed
		else {
			SPALog::log("Error: Parent arg1 stmt, arg2 wrong type");
		}
	}
	else {
		SPALog::log("Error: Parent arg1 wrong type");
	}

}

ResultTable QueryEvaluator::processFollows(vector<string> tempString) {
	
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	//cout << "arg1: "<<arg1Type << "arg2: "  << arg2Type << endl;
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
		else if (arg1Type == "while" || arg1Type == "assign" || arg1Type == "all") {
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
			else if (arg1Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
				if (isInList(stmtList, brother)) {
					temp.push_back(brother);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			else {
				cerr << "arg1 input err" << endl;
			}
			return tempResult;
		}
	}
	else if (arg2Type == "while" || arg2Type == "assign" || arg2Type == "all") {
		
		if (arg1Type == "prog_line") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			list<int> whileList = pkb.getWhileList();
			list<int> assignList = pkb.getAssignList();
			
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
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
				if (isInList(stmtList, rightSibling)) {
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
			list<int> whileList = pkb.getWhileList();
			list<int> assignList = pkb.getAssignList();
			if (arg2Type == "while") {
				if (arg1 == arg2) {
					tempResult.isWholeTrue = 0;
					return tempResult;
				}
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
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(),assignList.begin(), assignList.end());
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					if (rightSibling != -1 && isInList(stmtList, rightSibling)) {
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
			list<int> whileList = pkb.getWhileList();
			list<int> assignList = pkb.getAssignList();
			if (arg2Type == "while") {
				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
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
				if (arg1 == arg2) {
					tempResult.isWholeTrue = 0;
					return tempResult;
				}
				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					if (rightSibling != -1 && isInList(assignList, rightSibling)) {
						temp.push_back(*t);
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
 				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					if (rightSibling != -1 && isInList(stmtList, rightSibling)) {
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

ResultTable QueryEvaluator::processParentStar(vector<string> tempString) {
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	AST ast = pkb.getAST();
	if (arg2Type == "prog_line") {

		if (arg1Type == "prog_line") {//means result table should reaturn 1 or 0;
			ResultTable tempResult = ResultTable();
			int parent = ast.getParent(stoi(arg2));
			while (parent != -1) {
				if (parent == stoi(arg1)) {
					tempResult.isWholeTrue = 1;
					return tempResult;
				}
				parent = ast.getParent(parent);
			}
			tempResult.isWholeTrue = 0;
			return tempResult;
		}
		else if (arg1Type == "while" || arg1Type == "all") {
			ResultTable tempResult = ResultTable(arg1);
			int parent = ast.getParent(stoi(arg2));
			vector<int> temp;
			while (parent != -1) {
				// note to Zhao Hang: you are converting a letter to int
				// arg1 is always a letter or _
				//if (parent == stoi(arg1)) {
					temp.push_back(parent);
					tempResult.addTuple(temp);
					temp.clear();
				//}
				parent = ast.getParent(parent);
			}
			return tempResult;
		}
		else {
			cerr << "Parent* arg1Type undefined" << endl;
			return ResultTable();
		}
	}
	else if (arg2Type == "while" || arg2Type == "assign" || arg2Type == "all") {
		list<int> whileList = pkb.getWhileList();
		list<int> assignList = pkb.getAssignList();
		if (arg1Type == "prog_line") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			if (find(whileList.begin(), whileList.end(), stoi(arg1)) == whileList.end()) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			
			if (arg2Type == "while") {
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					int parent = ast.getParent(*t);
					while (parent != -1) {
						if (parent == stoi(arg1)) {
							temp.push_back(*t);
							tempResult.addTuple(temp);
							temp.clear();
						}
						parent = ast.getParent(parent);
					}
				}
			}
			else if (arg2Type == "assign") {
				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
					int parent = ast.getParent(*t);
					while (parent != -1) {
						if (parent == stoi(arg1)) {
							temp.push_back(*t);
							tempResult.addTuple(temp);
							temp.clear();
						}
						parent = ast.getParent(parent);
					}
				}
			}
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
				for (list<int>::iterator t = stmtList.begin(); t != stmtList.end(); t++) {
					int parent = ast.getParent(*t);
					while (parent != -1) {
						if (parent == stoi(arg1)) {
							temp.push_back(*t);
							tempResult.addTuple(temp);
							temp.clear();
						}
						parent = ast.getParent(parent);
					}
				}
			}
			else {
				cerr << "arg2 type err" << endl;
			}
			return tempResult;
		}
		else if (arg1Type == "while") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg2Type == "while") {
				if (arg1 == arg2) {
					tempResult.isWholeTrue = 0;
					return tempResult;
				}
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					for (list<int>::iterator i = whileList.begin(); i != whileList.end(); i++) {
						int parent = ast.getParent(*i);
						while (parent != -1) {
							if (parent == *t) {
								temp.push_back(*t);
								temp.push_back(*i);
								tempResult.addTuple(temp);
								temp.clear();
							}
							parent = ast.getParent(parent);
						}
					}
				}
			}
			else if (arg2Type == "assign") {
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
						int parent = ast.getParent(*i);
						while (parent != -1) {	
							if (parent == *t) {
								temp.push_back(*t);
								temp.push_back(*i);
								tempResult.addTuple(temp);
								temp.clear();
							}
							parent = ast.getParent(parent);
						}
					}
				}
			}
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
 				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
						int parent = ast.getParent(*i);
						while (parent != -1) {
							if (parent == *t) {
								temp.push_back(*t);
								temp.push_back(*i);
								tempResult.addTuple(temp);
								temp.clear();
							}
							parent = ast.getParent(parent);
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

ResultTable QueryEvaluator::processFollowsStar(vector<string> tempString) {
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
			int brother = ast.getFollowBefore(stoi(arg2));
			while (brother != -1) {
				if (brother == stoi(arg1)) {
					tempResult.isWholeTrue = 1;
					return tempResult;
				}
				int temp = ast.getFollowBefore(brother);
				brother = temp;
			}
			tempResult.isWholeTrue = 0;
			return tempResult;
		}
		else if (arg1Type == "while" || arg1Type == "assign" || arg1Type == "all") {
			ResultTable tempResult = ResultTable(arg1);
			int brother = ast.getFollowBefore(stoi(arg2));
			if (brother == -1) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			vector<int> temp;
			if (arg1Type == "while") {
				int brother = ast.getFollowBefore(stoi(arg2));
				while (brother != -1) {
					if (isInList(whileList, brother)) {
						temp.push_back(brother);
						tempResult.addTuple(temp);
						temp.clear();
					}
					brother = ast.getFollowBefore(brother);
				}
			}
			else if (arg1Type == "assign") {
				int brother = ast.getFollowBefore(stoi(arg2));
				while (brother != -1) {
					if (isInList(assignList, brother)) {
						temp.push_back(brother);
						tempResult.addTuple(temp);
						temp.clear();
					}
					brother = ast.getFollowBefore(brother);
				}
			}
			else if (arg1Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
 				int brother = ast.getFollowBefore(stoi(arg2));
				while (brother != -1) {
					if (isInList(stmtList, brother)) {
						temp.push_back(brother);
						tempResult.addTuple(temp);
						temp.clear();
					}
					brother = ast.getFollowBefore(brother);
				}
			}
			else {
				cerr << "arg1 inut err" << endl;
			}
			return tempResult;
		}
	}
	else if (arg2Type == "while" || arg2Type == "assign" || arg2Type == "all") {
		list<int> whileList = pkb.getWhileList();
		list<int> assignList = pkb.getAssignList();
		if (arg1Type == "prog_line") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			int rightSibling = ast.getFollowAfter(stoi(arg1));
			if (rightSibling == -1) {
				tempResult.isWholeTrue = 0;
				return tempResult;
			}
			if (arg2Type == "while") {
				int rightSibling = ast.getFollowAfter(stoi(arg1));
				while (rightSibling != -1) {
					if (isInList(whileList, rightSibling)) {
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
					rightSibling = ast.getFollowAfter(rightSibling);
				}
			}
			else if (arg2Type == "assign") {
				int rightSibling = ast.getFollowAfter(stoi(arg1));
				while (rightSibling != -1) {
					if (isInList(assignList, rightSibling)) {
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
					rightSibling = ast.getFollowAfter(rightSibling);
				}
			}
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
				int rightSibling = ast.getFollowAfter(stoi(arg1));
				while (rightSibling != -1) {
					if (isInList(stmtList, rightSibling)) {
						temp.push_back(rightSibling);
						tempResult.addTuple(temp);
						temp.clear();
					}
					rightSibling = ast.getFollowAfter(rightSibling);
				}
			}
			else {
				cerr << "arg2Type wrong" << endl;
			}
			return tempResult;
		}
		else if (arg1Type == "while") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg2Type == "while") {
				if (arg1 == arg2) {
					tempResult.isWholeTrue = 0;
					return tempResult;
				}
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					while (rightSibling != -1) {
						if (isInList(whileList, rightSibling)) {
							temp.push_back(*t);
							temp.push_back(rightSibling);
							tempResult.addTuple(temp);
							temp.clear();
						}
						rightSibling = ast.getFollowAfter(rightSibling);
					}
				}
			}
			else if (arg2Type == "assign") {
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					while (rightSibling != -1) {
						if (isInList(assignList, rightSibling)) {
							temp.push_back(*t);
							temp.push_back(rightSibling);
							tempResult.addTuple(temp);
							temp.clear();
						}
						rightSibling = ast.getFollowAfter(rightSibling);
					}
				}
			}
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
				for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					while (rightSibling != -1) {
						if (isInList(stmtList, rightSibling)) {
							temp.push_back(*t);
							temp.push_back(rightSibling);
							tempResult.addTuple(temp);
							temp.clear();
						}
						rightSibling = ast.getFollowAfter(rightSibling);
					}
				}
			}
			return tempResult;
		}
		else if (arg1Type == "assign") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg2Type == "while") {
				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					while (rightSibling != -1) {
						if (isInList(whileList, rightSibling)) {
							temp.push_back(*t);
							temp.push_back(rightSibling);
							tempResult.addTuple(temp);
							temp.clear();
						}
						rightSibling = ast.getFollowAfter(rightSibling);
					}
				}
			}
			else if (arg2Type == "assign") {
				if (arg1 == arg2) {
					tempResult.isWholeTrue = 0;
					return tempResult;
				}
				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					while (rightSibling != -1) {
						if (isInList(assignList, rightSibling)) {
							temp.push_back(*t);
							temp.push_back(rightSibling);
							tempResult.addTuple(temp);
							temp.clear();
						}
						rightSibling = ast.getFollowAfter(rightSibling);
					}
				}
			}
			else if (arg2Type == "all") {
				list<int> stmtList = whileList;
				stmtList.insert(stmtList.end(), assignList.begin(), assignList.end());
				for (list<int>::iterator t = assignList.begin(); t != assignList.end(); t++) {
					int rightSibling = ast.getFollowAfter(*t);
					while (rightSibling != -1) {
						if (isInList(stmtList, rightSibling)) {
							temp.push_back(*t);
							temp.push_back(rightSibling);
							tempResult.addTuple(temp);
							temp.clear();
						}
						rightSibling = ast.getFollowAfter(rightSibling);
					}
				}
			}
			return tempResult;
		}
		else {
			cerr << "arg1Type" << endl;
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
