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
QueryEvaluator::QueryEvaluator() {
	queryTree = QueryTree();
}

QueryEvaluator::QueryEvaluator(QueryTree qt) {
	queryTree = qt;
}
// entry function for controller;
list<string> QueryEvaluator::evaluate() {

	// first evaluate Const Const
	int index;
	Clause selectClause = queryTree.getSelectTree().at(0);

	for (vector<Clause>::iterator i = queryTree.getWithNoVarTree().begin(); i != queryTree.getWithNoVarTree().end(); i++) {
		if (!processWithConstClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	for (vector<Clause>::iterator i = queryTree.getSuchThatNoVarTree().begin(); i != queryTree.getSuchThatNoVarTree().end(); i++) {
		if (!processSuchThatConstClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	
	//remove the queries not related to select
	if (selectClause.getVarType().at(0) != "boolean") {
		indexQueryTree();
		vector<string> tempHeaderList;
		vector<Clause> clauseList;
		list<int> group1;

		tempHeaderList.insert(tempHeaderList.end(), selectClause.getVar().begin(), selectClause.getVar().end());
		clauseList.insert(clauseList.end(), queryTree.getPatternOneVarTree().begin(), queryTree.getPatternOneVarTree().end());
		clauseList.insert(clauseList.end(), queryTree.getSuchThatOneVarTree().begin(), queryTree.getSuchThatOneVarTree().end());
		clauseList.insert(clauseList.end(), queryTree.getWithOneVarTree().begin(), queryTree.getWithOneVarTree().end());
		clauseList.insert(clauseList.end(), queryTree.getPatternTwoVarTree().begin(), queryTree.getPatternTwoVarTree().end());
		clauseList.insert(clauseList.end(), queryTree.getSuchThatTwoVarTree().begin(), queryTree.getSuchThatTwoVarTree().end());
		clauseList.insert(clauseList.end(), queryTree.getWithTwoVarTree().begin(), queryTree.getWithTwoVarTree().end());


		int tempHeaderIndex = 0;
		list<int> evaluateGroup;
		size_t clauseListSize = clauseList.size();

		while (true) {
			if (clauseList.empty()) {
				break;
			}

			for (vector<Clause>::iterator it = clauseList.begin(); it != clauseList.begin(); it++) {
				bool isDelete = false;
				for (int i = 0; i < (*it).getVar().size(); ++i) {
					if (((*it).getVar().at(i) != "string" && (*it).getVar().at(i) != "number") && (*it).getVar().at(i) == tempHeaderList.at(tempHeaderIndex)) {
						evaluateGroup.push_back((*it).getIndex());
						if ((i == 0 && (*it).getVar().at(1) != "string" && (*it).getVar().at(1) != "number") ||
							(i == 1 && (*it).getVar().at(0) != "string" && (*it).getVar().at(0) != "number")) {
							string variable;
							if (i==0) {
								variable = (*it).getVar().at(1);
							}
							else {
								variable = (*it).getVar().at(0);
							}

							if (find(tempHeaderList.begin(), tempHeaderList.end(), variable)
								== tempHeaderList.end()) {
								tempHeaderList.push_back(variable);
							}
						}
						isDelete = true;
						break;
					}
				}

				if (isDelete) {
					clauseList.erase(it++);
				}
				else {
					++it;
				}
			}
			if (clauseListSize == clauseList.size()) {
				break;
			}
			else {
				clauseListSize = clauseList.size();
				++tempHeaderIndex;
			}
		}

		//evaluate group generated, then run through the query tree, check whether inside evaluated group, if not boolEvaluate it, if true, remove, else return none
		for (vector<Clause>::iterator i = queryTree.getPatternOneVarTree().begin(); i != queryTree.getPatternOneVarTree().end(); i++) {
			if (!isInList(evaluateGroup, (*i).getIndex())) {
				if (!processPatternClause(*i)) {
					list<string> empty;
					return empty;
				}
				else {
					queryTree.getPatternOneVarTree().erase(i);
				}
			}
		}
		for (vector<Clause>::iterator i = queryTree.getSuchThatOneVarTree().begin(); i != queryTree.getSuchThatOneVarTree().end(); i++) {
			if (!isInList(evaluateGroup, (*i).getIndex())) {
				if (!processSuchThatClause(*i)) {
					list<string> empty;
					return empty;
				}
				else {
					queryTree.getSuchThatOneVarTree().erase(i);
				}
			}
		}
		for (vector<Clause>::iterator i = queryTree.getWithOneVarTree().begin(); i != queryTree.getWithOneVarTree().end(); i++) {
			if (!isInList(evaluateGroup, (*i).getIndex())) {
				if (!processWithClause(*i)) {
					list<string> empty;
					return empty;
				}
				else {
					queryTree.getWithOneVarTree().erase(i);
				}
			}
		}
		for (vector<Clause>::iterator i = queryTree.getPatternTwoVarTree().begin(); i != queryTree.getPatternTwoVarTree().end(); i++) {
			if (!isInList(evaluateGroup, (*i).getIndex())) {
				if (!processPatternClause(*i)) {
					list<string> empty;
					return empty;
				}
				else {
					queryTree.getPatternTwoVarTree().erase(i);
				}
			}
		}
		for (vector<Clause>::iterator i = queryTree.getSuchThatTwoVarTree().begin(); i != queryTree.getSuchThatTwoVarTree().end(); i++) {
			if (!isInList(evaluateGroup, (*i).getIndex())) {
				if (!processSuchThatClause(*i)) {
					list<string> empty;
					return empty;
				}
				else {
					queryTree.getSuchThatTwoVarTree().erase(i);
				}
			}
		}
		for (vector<Clause>::iterator i = queryTree.getWithTwoVarTree().begin(); i != queryTree.getWithTwoVarTree().end(); i++) {
			if (!isInList(evaluateGroup, (*i).getIndex())) {
				if (!processWithClause(*i)) {
					list<string> empty;
					return empty;
				}
				else {
					queryTree.getWithTwoVarTree().erase(i);
				}
			}
		}

	}
	else {

	}

	// start to evaluate
	// first evaluate one syn clauses
	for (vector<Clause>::iterator i = queryTree.getWithOneVarTree().begin(); i != queryTree.getWithOneVarTree().end(); i++) {
		if (!processWithClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	for (vector<Clause>::iterator i = queryTree.getSuchThatOneVarTree().begin(); i != queryTree.getSuchThatOneVarTree().end(); i++) {
		if (!processSuchThatClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	for (vector<Clause>::iterator i = queryTree.getPatternOneVarTree().begin(); i != queryTree.getPatternOneVarTree().end(); i++) {
		if (!processPatternClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	for (vector<Clause>::iterator i = queryTree.getWithTwoVarTree().begin(); i != queryTree.getWithTwoVarTree().end(); i++) {
		if (!processWithClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	for (vector<Clause>::iterator i = queryTree.getSuchThatTwoVarTree().begin(); i != queryTree.getSuchThatTwoVarTree().end(); i++) {
		if (!processSuchThatClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	for (vector<Clause>::iterator i = queryTree.getPatternTwoVarTree().begin(); i != queryTree.getPatternTwoVarTree().end(); i++) {
		if (!processPatternClause(*i)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}

	for (int i = 0; i < resultList.size(); ++i) {
		resultList[i].logTable(i);
	}

	
	QueryResultProjector qrp = QueryResultProjector(resultList, selectClause.getVar(), selectClause.getVarType());
	return qrp.getResult();
}

void QueryEvaluator::indexQueryTree() {
	int counter = 0;
	for (vector<Clause>::iterator i = queryTree.getPatternOneVarTree().begin(); i != queryTree.getPatternOneVarTree().end(); i++) {
		(*i).setIndex(counter);
		counter++;
	}
	for (vector<Clause>::iterator i = queryTree.getSuchThatOneVarTree().begin(); i != queryTree.getSuchThatOneVarTree().end(); i++) {
		(*i).setIndex(counter);
		counter++;
	}
	for (vector<Clause>::iterator i = queryTree.getWithOneVarTree().begin(); i != queryTree.getWithOneVarTree().end(); i++) {
		(*i).setIndex(counter);
		counter++;
	}
	for (vector<Clause>::iterator i = queryTree.getPatternTwoVarTree().begin(); i != queryTree.getPatternTwoVarTree().end(); i++) {
		(*i).setIndex(counter);
		counter++;
	}
	for (vector<Clause>::iterator i = queryTree.getSuchThatTwoVarTree().begin(); i != queryTree.getSuchThatTwoVarTree().end(); i++) {
		(*i).setIndex(counter);
		counter++;
	}
	for (vector<Clause>::iterator i = queryTree.getWithTwoVarTree().begin(); i != queryTree.getWithTwoVarTree().end(); i++) {
		(*i).setIndex(counter);
		counter++;
	}
}

//Process Clause
bool QueryEvaluator::processSuchThatClause(Clause tempString) {
	string relationship = tempString.getRelationship();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string log = "Such that clause: " + relationship + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (relationship == "modifies") {
		tempResult = processModifies(tempString);
	}
	else if (relationship == "uses") {
		tempResult = processUses(tempString);
	}
	else if (relationship == "parent") {
		tempResult = processParent(tempString);
	}
	else if (relationship == "follows") {
		tempResult = processFollows(tempString);
	}
	else if (relationship == "parent*") {
		tempResult = processParentStar(tempString);
	}
	else if (relationship == "follows*") {
		tempResult = processFollowsStar(tempString);
	}
	else if (relationship == "calls") {
		tempResult = processCalls(tempString);
	}
	else if (relationship == "calls*") {
		tempResult = processCallsStar(tempString);
	}
	else if (relationship == "next") {
		tempResult = processNext(tempString);
	}
	else if (relationship == "next*") {
		tempResult = processNextStar(tempString);
	}
	else {
		SPALog::log("Wrong relationship!");
		return false;
	}
	if (isResultEmpty(tempResult)) {
		return false;
	}
	resultList.push_back(tempResult);
	return true;
}

bool QueryEvaluator::processSuchThatConstClause(Clause tempString) {
	string relationship = tempString.getRelationship();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType.at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType.at(1);
	string log = "Such that const clause: " + relationship + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (relationship == "modifies") {
		tempResult = processModifies(tempString);
	}
	else if (relationship == "uses") {
		tempResult = processUses(tempString);
	}
	else if (relationship == "parent") {
		tempResult = processParent(tempString);
	}
	else if (relationship == "follows") {
		tempResult = processFollows(tempString);
	}
	else if (relationship == "parent*") {
		tempResult = processParentStar(tempString);
	}
	else if (relationship == "follows*") {
		tempResult = processFollowsStar(tempString);
	}
	else if (relationship == "calls") {
		tempResult = processCalls(tempString);
	}
	else if (relationship == "calls*") {
		tempResult = processCallsStar(tempString);
	}
	else if (relationship == "next") {
		tempResult = processNext(tempString);
	}
	else if (relationship == "next*") {
		tempResult = processNextStar(tempString);
	}
	else {
		SPALog::log("Wrong relationship!");
		return false;
	}
	if (tempResult.getIsWholeTrue()==0) {
		return false;
	}
	else { 
		return true;
	}
}

bool QueryEvaluator::isInList(list<int> list, int number) {
	if (find(list.begin(), list.end(), number) != list.end()) {
		return true;
	}
	return false;
}

bool QueryEvaluator::isResultEmpty(ResultTable tempResult) {
	return tempResult.getContent().empty();
}

list<int> QueryEvaluator::getList(string arr, string arrType) {
	//find in intermediate result first
	for (vector<ResultTable>::iterator it = midResult.begin(); it != midResult.end(); ++it) {
		if ((*it).getHeader().at(0) == arr) {
			list<int> temp;
			for (vector<vector<int>>::iterator i = (*it).getContent().begin(); i != (*it).getContent().end(); ++i) {
				temp.push_back((*i).at(0));
			}
			return temp;
		}
	}
	//if not in the intermediate result
	if (arrType == "assign") {
		return PKB::getPKBInstance()->getAssignList();
	}
	else if (arrType == "while") {
		return PKB::getPKBInstance()->getWhileList();
	}
	else if (arrType == "if") {
		return PKB::getPKBInstance()->getIfList();
	}
	else if (arrType == "call") {
		return PKB::getPKBInstance()->getCallList();
	}
	else if (arrType == "stmt" || arrType == "all" || arrType == "prog_line") {
		return PKB::getPKBInstance()->getStmtList();
	}
	else if (arrType == "procedure") {
		return PKB::getPKBInstance()->getProcList();
	}
	else if (arrType == "constant") {
		return PKB::getPKBInstance()->getConstantList();
	}
	else if (arrType == "variable") {
		return PKB::getPKBInstance()->getVarList();
	}
	else {
		list<int> emptyList;
		SPALog::log("Wrong list type!");
		return emptyList;
	}
}

void QueryEvaluator::_updateMidResult(ResultTable newResult) {
	//firstly split them and unify them
	if (newResult.getHeader().size() == 2) {
		ResultTable r1 = ResultTable(newResult.getHeader().at(0));
		ResultTable r2 = ResultTable(newResult.getHeader().at(1));
		bool inside;
		for (vector<vector<int>>::iterator i = newResult.getContent().begin(); i != newResult.getContent().end(); ++i) {
			inside = false;
			for (vector<vector<int>>::iterator t = r1.getContent().begin(); t != r1.getContent().end(); ++t) {
				if ((*i).at(0) == (*t).at(0)) {
					inside = true;
				}
			}
			if (!inside) {
				vector<int> temp;
				temp.push_back((*i).at(0));
				r1.addTuple(temp);
			}
		}
		for (vector<vector<int>>::iterator i = newResult.getContent().begin(); i != newResult.getContent().end(); ++i) {
			inside = false;
			for (vector<vector<int>>::iterator t = r1.getContent().begin(); t != r1.getContent().end(); ++t) {
				if ((*i).at(1) == (*t).at(0)) {
					inside = true;
				}
			}
			if (!inside) {
				vector<int> temp;
				temp.push_back((*i).at(1));
				r2.addTuple(temp);
			}
		}
		updateMidResult(r1);
		updateMidResult(r2)
	}
	else {
		updateMidResult(newResult);
	}
}

void QueryEvaluator::updateMidResult(ResultTable newResult) {
	for (vector<ResultTable>::iterator it = midResult.begin(); it != midResult.end(); ++it) {
		if ((*it).getHeader().at(0) == newResult.getHeader().at(0)) {
			//merge these two 
			bool findIt;
			for (vector<vector<int>>::iterator i = (*it).getContent().begin(); i != (*it).getContent().end(); ++i) {
				findIt = false;
				for (vector<vector<int>>::iterator t = newResult.getContent().begin(); t != newResult.getContent().end(); ++t) {
					if ((*i).at(0) == (*t).at(0)) {
						//find it 
						findIt = true;
					}
				}
				if (!findIt) {
					(*it).getContent().erase(i);
				}
			}
			
		}
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
			tempResult.setIsWholeTrue(0);
			SPALog::log("arg2 is not a valid variable!\n");
			return tempResult;
		}

		list<int> modifiesLine = PKB::getPKBInstance()->getModifiesFirst(arg2ID);

		if ( arg1Type == "number" ) {
			ResultTable tempResult = ResultTable();

			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isModifiesValid(stoi(arg1), arg2ID)) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if ( arg1Type == "string" ) {
			ResultTable tempResult = ResultTable();
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
			if (!arg1ID) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg1 is not a valid proc_name!\n");
				return tempResult;
			}
			
			if (find(modifiesLine.begin(), modifiesLine.end(), arg1ID) != modifiesLine.end()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg1 is procedure, stmt, while, assign, if, call, prog_line
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
		
		if (arg1Type == "number") {
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
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
		else if (arg1Type == "string") {
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
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
			//procedue, while, assign, if, call, stmt, prog_line
			list<int> targetList = getList(arg1Type);
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
			tempResult.setIsWholeTrue(0);
			SPALog::log("arg2 is not a valid variable!\n");
			return tempResult;
		}

		list<int> usesLine = PKB::getPKBInstance()->getUsesFirst(arg2ID);

		if (arg1Type == "number") {
			ResultTable tempResult = ResultTable();

			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isUsesValid(stoi(arg1), arg2ID)) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if (arg1Type == "string") {
			ResultTable tempResult = ResultTable();
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
			if (!arg1ID) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg1 is not a valid proc_name!\n");
				return tempResult;
			}

			if (find(usesLine.begin(), usesLine.end(), arg1ID) != usesLine.end()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg2 is procedure, stmt, while, assign, if, call, prog_line
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
		if (arg1Type == "number") {
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
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
		else if (arg1Type == "string") {
			int arg1ID = PKB::getPKBInstance()->getProcID(arg1);
			if (!arg1ID) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
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

			list<int> targetList = getList(arg1Type);
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

	if (arg1Type == "number") {
		bool isNotParrent = !isInList(parentList, stoi(arg1));
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1)) || isNotParrent ) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Parent arg1 is not a valid prog_line or is not a parent stmt");
			return tempResult;
		}

		list<int> childList = PKB::getPKBInstance()->getParentSecond(stoi(arg1));

		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Parent arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isParentValid(stoi(arg1), stoi(arg2))) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			//stmt, assign, while, call, if, prog_line, all
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
		// arg1Type can be while, if, stmt, prog_line, all
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
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
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			//arg2Type can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1,arg2);
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
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
	
	if (arg1Type == "number") {
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Follows arg1 is not a valid prog_line");
			return tempResult;
		}

		int littleBrother = PKB::getPKBInstance()->getFollowsSecond(stoi(arg1));
		if (littleBrother == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			return tempResult;
		}
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Follows arg2 is not a valid prog_line");
				return tempResult;
			}
			if ( PKB::getPKBInstance()->isFollowsValid(stoi(arg1), stoi(arg2))){
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt, prog_line, all
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
		// arg1 can be while, assign, call, stmt, if, prog_line, all
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
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
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
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
	
	if (arg1Type == "number") {
		bool isNotParrent = !isInList(parentList, stoi(arg1));
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1)) || isNotParrent) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Parent* arg1 is not a valid prog_line or is not a parent stmt");
			return tempResult;
		}

		list<int> childList = PKB::getPKBInstance()->getParentStarSecond(stoi(arg1));

		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Parent* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isParentStarValid(stoi(arg1), stoi(arg2))) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			//stmt, assign, while, call, if, prog_line, all
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
		// arg1Type can be while, if, stmt, prog_line, all
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
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
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			//arg2Type can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
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

	if (arg1Type == "number") {
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Follows* arg1 is not a valid prog_line");
			return tempResult;
		}

		list<int> littleBrothers = PKB::getPKBInstance()->getFollowsStarSecond(stoi(arg1));
		if (littleBrothers.size() == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Follows* arg1 does not have little brothers");
			return tempResult;
		}
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Follows* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isFollowsStarValid(stoi(arg1), stoi(arg2))) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt,prog_line, all
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
		// arg1 can be while, assign, call, stmt, if,prog_line, all
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
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
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
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
	if (arg1Type == "string") {
		int arg1ID = PKB::getPKBInstance()->getProcID(arg1);

		if (!arg1ID) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("arg1 is not a valid proc id!\n");
			return tempResult;
		}

		list<int> procedureCalled = PKB::getPKBInstance()->getCallsSecond(arg1ID);

		if (arg2Type == "string") {
			ResultTable tempResult = ResultTable();
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID || (arg1 == arg2)) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg2 is not a valid proc id or arg1 == arg2!\n");
				return tempResult;
			}

			if (PKB::getPKBInstance()->isCallsValid(arg1ID, arg2ID)) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
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
		if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.setIsWholeTrue(0);
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
			if (arg1Type != "all" && arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Calls invalid arg1 == arg2!\n");
				return tempResult;
			}

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
	if (arg1Type == "string") {
		int arg1ID = PKB::getPKBInstance()->getProcID(arg1);

		if (!arg1ID) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("arg1 is not a valid proc id!\n");
			return tempResult;
		}

		list<int> procedureCalled = PKB::getPKBInstance()->getCallsStarSecond(arg1ID);

		if (arg2Type == "string") {
			ResultTable tempResult = ResultTable();
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID || arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg2 is not a valid proc id! or arg1 == arg2\n");
				return tempResult;
			}

			if (PKB::getPKBInstance()->isCallsStarValid(arg1ID, arg2ID)) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
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
		if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.setIsWholeTrue(0);
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
			if (arg1Type != "all" && arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Calls* invalid arg1 == arg2!\n");
				return tempResult;
			}

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

ResultTable QueryEvaluator::processNext(vector<string> tempString){
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);

	if (arg1Type == "number") {
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Next arg1 is not a valid prog_line");
			return tempResult;
		}

		list<int> littleBrothers = PKB::getPKBInstance()->getNextSecond(stoi(arg1));
		if (littleBrothers.size() == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			return tempResult;
		}
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Next arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isNextvalid(stoi(arg1), stoi(arg2))) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt, prog_line, all
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
		// arg1 can be while, assign, call, stmt, if, prog_line, all
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Next arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> brothers = PKB::getPKBInstance()->getNextFirst(stoi(arg2));
			vector<int> temp;
			if (brothers.size() == 0) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
				return tempResult;
			}
			for (list<int>::iterator t = brothers.begin(); t != brothers.end(); t++) {
				if (isInList(arg1List, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
				return tempResult;
			}
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getNextSecond(*t);
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

ResultTable QueryEvaluator::processNextStar(vector<string> tempString) {
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);

	if (arg1Type == "number") {
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Next* arg1 is not a valid prog_line");
			return tempResult;
		}

		list<int> littleBrothers = PKB::getPKBInstance()->getNextStarSecond(stoi(arg1));
		if (littleBrothers.size() == 0) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Next* arg1 does not have little brothers");
			return tempResult;
		}
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Next* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isNextStarValid(stoi(arg1), stoi(arg2))) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt, prog_line, all
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
		// arg1 can be while, assign, call, stmt, if, prog_line, all
		list<int> arg1List = getList(arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Next* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> brothers = PKB::getPKBInstance()->getNextStarFirst(stoi(arg2));
			vector<int> temp;
			if (brothers.size() != 0) {
				for (list<int>::iterator t = brothers.begin(); t != brothers.end(); t++) {
					if (isInList(arg1List, *t)) {
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
				return tempResult;
			}
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getNextStarSecond(*t);
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

bool QueryEvaluator::processPatternClause(Clause tempString) {
	
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string syn = tempString.getVar().at(2);
	string synType = tempString.getVarType().at(2);
	
	string log = "Pattern clause: " + synType + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (synType == "assign") {
		tempResult = processAssignPattern(tempString);
	}
	else if (synType == "while") {
		tempResult = processWhilePattern(tempString);
	}
	else if (synType == "if") {
		tempResult = processIfPattern(tempString);
	}
	else {
		SPALog::log("Wrong pattern!");
		return false;
	}

	if (isResultEmpty(tempResult)) {
		return false;
	}
	resultList.push_back(tempResult);
	return true;
	
}

ResultTable QueryEvaluator::processAssignPattern(Clause tempString) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string syn = tempString.getVar().at(2);
	string synType = tempString.getVarType().at(2);

	if (arg1Type == "string") {
		// PatternOneVarTree
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
		else if (arg2Type == "string") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithBothExact(arg1, arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		else if (arg2Type == "substring") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithBoth(arg1, arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		else {
			SPALog::log("Pattern arg2 wrong type");
		}
		_updateMidResult(tempResult);
		return tempResult;
	}
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(syn, arg1);
		vector<int> temp;
		if (arg2Type == "all") {
			list<int> varList = getList(arg1,arg1Type);
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
		else if (arg2Type == "string") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecondExact(arg2);
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
		else if (arg2Type == "substring") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecond(arg2);
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
		else {
			SPALog::log("Pattern arg2 wrong type");
		}
		_updateMidResult(tempResult);
		return tempResult;
	}
	else if (arg1Type == "all") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		if (arg2Type == "all") {
			list<int> assignList = getList(syn, synType);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		else if (arg2Type == "string") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecondExact(arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		else if (arg2Type == "substring") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecond(arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
		}
		else {
			SPALog::log("Pattern arg2 wrong type");
		}
		_updateMidResult(tempResult);
		return tempResult;
	}
	else {
		SPALog::log("pattern arg1 type wrong");
		return ResultTable();
	}
}

ResultTable QueryEvaluator::processWhilePattern(Clause tempString) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string syn = tempString.getVar().at(2);
	string synType = tempString.getVarType().at(2);

	if (arg1Type == "string") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		int arg1ID = PKB::getPKBInstance()->getVarID(arg1);
		if (!arg1ID) {
			tempResult.setIsWholeTrue(0);
			SPALog::log("arg1 is not a valid variable!\n");
			return tempResult;
		}

		list<int> whileList = PKB::getPKBInstance()->getWhileWithFirstExact(arg1);
		for (list<int>::iterator i = whileList.begin(); i != whileList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		_updateMidResult(tempResult);
		return tempResult;
	}
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(syn, arg1);
		vector<int> temp;
		list<int> varList = getList(arg1, arg1Type);
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
		_updateMidResult(tempResult);
		return tempResult;
	}
	else {
	 //arg1Type == all
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		
		list<int> whileList = getList(syn, synType);
		for (list<int>::iterator t = whileList.begin(); t != whileList.end(); t++) {
			temp.push_back(*t);
			tempResult.addTuple(temp);
			temp.clear();
		}
		_updateMidResult(tempResult);
		return tempResult;
	}
}

ResultTable QueryEvaluator::processIfPattern(Clause tempString) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string syn = tempString.getVar().at(2);
	string synType = tempString.getVarType().at(2);

	if (arg1Type == "string") {
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;
		int arg1ID = PKB::getPKBInstance()->getVarID(arg1);
		if (!arg1ID) {
			tempResult.setIsWholeTrue(0);
			SPALog::log("arg1 is not a valid variable!\n");
			return tempResult;
		}

		list<int> ifList = PKB::getPKBInstance()->getIfWithFirstExact(arg1);
		for (list<int>::iterator i = ifList.begin(); i != ifList.end(); i++) {
			temp.push_back(*i);
			tempResult.addTuple(temp);
			temp.clear();
		}
		_updateMidResult(tempResult);
		return tempResult;
	}
	else if (arg1Type == "variable") {
		ResultTable tempResult = ResultTable(syn, arg1);
		vector<int> temp;
		list<int> varList = getList(arg1, arg1Type);
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
		_updateMidResult(tempResult);
		return tempResult;
	}
	else {
		//arg1Type == all
		ResultTable tempResult = ResultTable(syn);
		vector<int> temp;

		list<int> ifList = getList(syn, synType);
		for (list<int>::iterator t = ifList.begin(); t != ifList.end(); t++) {
			temp.push_back(*t);
			tempResult.addTuple(temp);
			temp.clear();
		}
		_updateMidResult(tempResult);
		return tempResult;
	}
}

bool QueryEvaluator::processSelectClause(Clause tempString) {
	int tupleSize = tempString.size()/2;
	for (int i = 0; i < tupleSize; i++) {

		string syn = tempString.at(i);
		string synType = tempString.at(i + 1);
		string log = "Select clause: select " + syn + ":" + synType + "\n";
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
			if (isResultEmpty(tempResult)) {
				return false;
			}
			resultList.push_back(tempResult);
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
			if (isResultEmpty(tempResult)) {
				return false;
			}
			resultList.push_back(tempResult);
		}
		else if (synType == "boolean") {
			//ResultTable tempResult = ResultTable(syn);
			//tempResult.isWholeTrue = 1;
			//resultList.push_back(tempResult);
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
			if (isResultEmpty(tempResult)) {
				return false;
			}
			resultList.push_back(tempResult);
		}
	}
	return true;
}

bool QueryEvaluator::processWithClause(Clause tempString) {
	string synType = tempString.getRelationship();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

	string log = "With clause: " + synType + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (synType == "withNumber") {
		tempResult = processNumberWith(tempString);
	}
	else if (synType == "withName") {
		tempResult = processNameWith(tempString);
	}
	else {
		SPALog::log("Wrong with type!");
		return false;
	}

	if (isResultEmpty(tempResult)) {
		return false;
	}
	resultList.push_back(tempResult);
	return true;
}

bool QueryEvaluator::processWithConstClause(Clause tempString) {
	string synType = tempString.getRelationShip();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType.at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType.at(1);

	string log = "With constant clause: " + synType + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (synType == "withNumber") {
		tempResult = processNumberWith(tempString);
	}
	else if (synType == "withName") {
		tempResult = processNameWith(tempString);
	}
	else {
		SPALog::log("Wrong with type!");
		return false;
	}

	if (tempResult.getIsWholeTrue() == 0) {
		return false;
	}
	else {
		return true;
	}
	
}

ResultTable QueryEvaluator::processNameWith(vector<string> tempString) {
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	/*
	arg1 and arg2 can be: procedure, call, variable, string
	*/
	if (arg1Type == "procedure") {
		if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(1);
				return tempResult;
			}
			list<int> procList = getList("procedure");
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				temp.push_back(*i);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList("call");
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				//get call stmt's procedure id
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				temp.push_back(procID);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "variable") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> procList = getList("procedure");
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					string varName = PKB::getPKBInstance()->getVarName(*t);
					if (procName == varName) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> procList = getList("procedure");
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				if (procName == arg2) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else {

		}
	}
	else if (arg1Type == "call") {
		if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList("call");
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				//get call stmt's procedure id
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				temp.push_back(*i);
				temp.push_back(procID);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(1);
				return tempResult;
			}
			vector<int> temp;
			list<int> callList = getList("call");
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int proc1 = PKB::getPKBInstance()->getCallStmtProc(*i);
				for (list<int>::iterator t = callList.begin(); t != callList.end(); t++) {
					int proc2 = PKB::getPKBInstance()->getCallStmtProc(*t);
					if (proc1 == proc2) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
					
			}
			return tempResult;
		}
		else if (arg2Type == "variable") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList("call");
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				string procName = PKB::getPKBInstance()->getProcName(procID);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					string varName = PKB::getPKBInstance()->getVarName(*t);
					if (procName == varName) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}

			}
			return tempResult;
		}
		else if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> callList = getList("call");
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				string procName = PKB::getPKBInstance()->getProcName(procID);
				if (procName == arg2) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else {

		}
	}
	else if (arg1Type == "variable") {
		if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> procList = getList("procedure");
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					string varName = PKB::getPKBInstance()->getVarName(*t);
					if (procName == varName) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList("call");
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				string procName = PKB::getPKBInstance()->getProcName(procID);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					string varName = PKB::getPKBInstance()->getVarName(*t);
					if (procName == varName) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
		else if (arg2Type == "variable") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(1);
				return tempResult;
			}
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				string varName = PKB::getPKBInstance()->getVarName(*i);
				if (varName == arg2) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else {

		}
	}
	else if (arg1Type == "string") {
		if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			list<int> procList = getList("procedure");
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				if (procName == arg1) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			list<int> callList = getList("call");
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				string procName = PKB::getPKBInstance()->getProcName(procID);
				if (procName == arg1) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg2Type == "variable") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			list<int> varList = PKB::getPKBInstance()->getVarList();
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				string varName = PKB::getPKBInstance()->getVarName(*i);
				if (varName == arg1) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
				}
			}
			return tempResult;
		}
		else if (arg2Type == "string") {
			ResultTable tempResult = ResultTable();
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {

		}
	}
	else {

	}

}

ResultTable QueryEvaluator::processNumberWith(vector<string> tempString) {
	string arg1 = tempString.at(1);
	string arg1Type = tempString.at(2);
	string arg2 = tempString.at(3);
	string arg2Type = tempString.at(4);
	//arg1 and arg2 can be: prog_line, number, constant, (stmt, call, while, assign, call)
	if (arg1Type == "prog_line") {
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(1);
				return tempResult;
			}
			list<int> stmtList = getList("stmt");
			for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
				temp.push_back(*i);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("with number clause arg2 is not a valid prog_line");
				return tempResult;
			}
			temp.push_back(stoi(arg2));
			tempResult.addTuple(temp);
			temp.clear();
			return tempResult;
		}
		else {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> targetList = getList(arg2Type);
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> arg2List = getList(arg2Type);
				for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
					if (*i == *t) {
						temp.push_back(*i);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
					}
				}
			}
			return tempResult;
		}
	}
	else if (arg1Type == "number") {
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("with number clause arg1 is not a valid prog_line");
				return tempResult;
			}
			temp.push_back(stoi(arg1));
			tempResult.addTuple(temp);
			temp.clear();
			return tempResult;
		}
		else if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			vector<int> temp;
			if ( stoi(arg1) != stoi(arg2)) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("with number clause arg1 != arg2");
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			
			return tempResult;
		}
		else {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			
			list<int> targetList = getList(arg2Type);
		
			if (isInList(targetList, stoi(arg1))) {
				temp.push_back(stoi(arg1));
				tempResult.addTuple(temp);
				temp.clear();
			}
			
			return tempResult;
		}
	}
	else {
		list<int> targetList = getList(arg1Type);
		//arg1 == stmt, call, while, assign, call, constant
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
		
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				temp.push_back(*i);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
			}
			return tempResult;
		}
		else if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;

			if (isInList(targetList, stoi(arg2))) {
				temp.push_back(stoi(arg2));
				tempResult.addTuple(temp);
				temp.clear();
			}

			return tempResult;
		}
		else {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(1);
				return tempResult;
			}
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> arg2List = getList(arg2Type);
				for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
					if (*i == *t) {
						temp.push_back(*i);
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

