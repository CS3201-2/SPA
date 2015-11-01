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

bool QueryEvaluator::processClause(Clause tempString, bool isUseful, bool noVar) {
	string rel = tempString.getRelationship();
	if (rel == "select") {
		return processSelectClause(tempString, isUseful);
	}
	else if (rel == "assign" || rel == "while" || rel == "if") {
		return processPatternClause(tempString, isUseful);
	}
	else if (rel =="withName" || rel =="withNumber") {
		if (noVar) {
			return processWithConstClause(tempString, isUseful);
		}
		else {
			return processWithClause(tempString, isUseful);
		}
	}
	else {
		if (noVar) {
			return processSuchThatConstClause(tempString, isUseful);
		}
		else {
			return processSuchThatClause(tempString, isUseful);
		}
	}
}

// entry function for controller;
list<string> QueryEvaluator::evaluate() {
	queryTree.grouping();
	// first evaluate useful no var query
	int index;
	Clause selectClause = queryTree.getSelectClause();

	vector<Clause> usefulNoVarTree = queryTree.getUsefulNoVarTree();
	vector<Clause> usefulOneVarTree = queryTree.getUsefulOneVarTree();
	vector<Clause> usefulTwoVarTree = queryTree.getUsefulTwoVarTree();
	vector<Clause> uselessOneVarTree = queryTree.getUselessOneVarTree();
	vector<Clause> uselessTwoVarTree = queryTree.getUselessTwoVarTree();
	string str;
	str += to_string(usefulNoVarTree.size());
	str += to_string(usefulOneVarTree.size());
	str += to_string(usefulTwoVarTree.size());
	str += to_string(uselessOneVarTree.size());
	str += to_string(uselessTwoVarTree.size());
	SPALog::log(str);

	//evaluate no var
	for (vector<Clause>::iterator i = usefulNoVarTree.begin(); i != usefulNoVarTree.end(); i++) {
		if (!processClause(*i, true, true)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	
	//evaluate useless clause
	for (vector<Clause>::iterator i = uselessOneVarTree.begin(); i != uselessOneVarTree.end(); i++) {
		if (!processClause(*i, false, false)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}

	for (vector<Clause>::iterator i = uselessTwoVarTree.begin(); i != uselessTwoVarTree.end(); i++) {
		if (!processClause(*i, false, false)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}

	// evaluate useful clause
	for (vector<Clause>::iterator i = usefulOneVarTree.begin(); i != usefulOneVarTree.end(); i++) {
		if (!processClause(*i, true, false)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}
	for (vector<Clause>::iterator i = usefulTwoVarTree.begin(); i != usefulTwoVarTree.end(); i++) {
		if (!processClause(*i, true, false)) {
			list<string> empty;
			if (selectClause.getVarType().at(0) == "boolean") {
				empty.push_back("false");
			}
			return empty;
		}
	}

	// select clause
	if (!processSelectClause(selectClause, true)) {
		list<string> empty;
		if (selectClause.getVarType().at(0) == "boolean") {
			empty.push_back("false");
		}
		return empty;
	}

	for (int i = 0; i < resultList.size(); ++i) {
		resultList[i].logTable(i);
	}

	QueryResultProjector qrp = QueryResultProjector(resultList, selectClause.getVar(), selectClause.getVarType());
	return qrp.getResult();
}

//Process Clause
bool QueryEvaluator::processSuchThatClause(Clause tempString, bool useful) {
	string relationship = tempString.getRelationship();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string log = "Such that clause: " + relationship + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (relationship == "modifies") {
		tempResult = processModifies(tempString, useful);
	}
	else if (relationship == "uses") {
		tempResult = processUses(tempString, useful);
	}
	else if (relationship == "parent") {
		tempResult = processParent(tempString, useful);
	}
	else if (relationship == "follows") {
		tempResult = processFollows(tempString, useful);
	}
	else if (relationship == "parent*") {
		tempResult = processParentStar(tempString, useful);
	}
	else if (relationship == "follows*") {
		tempResult = processFollowsStar(tempString, useful);
	}
	else if (relationship == "calls") {
		tempResult = processCalls(tempString, useful);
	}
	else if (relationship == "calls*") {
		tempResult = processCallsStar(tempString, useful);
	}
	else if (relationship == "next") {
		tempResult = processNext(tempString, useful);
	}
	else if (relationship == "next*") {
		tempResult = processNextStar(tempString, useful);
	}
	else if (relationship == "affects") {
		tempResult = processAffects(tempString, useful);
	}
	else if (relationship == "affects*") {
		tempResult = processAffectsStar(tempString, useful);
	}
	else {
		SPALog::log("Wrong relationship!");
		return false;
	}

	if (tempResult.getIsWholeTrue()==1) {
		return true;
	}
	if (isResultEmpty(tempResult)) {
		return false;
	}

	if (useful) {
		resultList.push_back(tempResult);
	}
	
	return true;
}

bool QueryEvaluator::processSuchThatConstClause(Clause tempString, bool useful) {
	string relationship = tempString.getRelationship();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string log = "Such that const clause: " + relationship + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (relationship == "modifies") {
		tempResult = processModifies(tempString, useful);
	}
	else if (relationship == "uses") {
		tempResult = processUses(tempString, useful);
	}
	else if (relationship == "parent") {
		tempResult = processParent(tempString, useful);
	}
	else if (relationship == "follows") {
		tempResult = processFollows(tempString, useful);
	}
	else if (relationship == "parent*") {
		tempResult = processParentStar(tempString, useful);
	}
	else if (relationship == "follows*") {
		tempResult = processFollowsStar(tempString, useful);
	}
	else if (relationship == "calls") {
		tempResult = processCalls(tempString, useful);
	}
	else if (relationship == "calls*") {
		tempResult = processCallsStar(tempString, useful);
	}
	else if (relationship == "next") {
		tempResult = processNext(tempString, useful);
	}
	else if (relationship == "next*") {
		tempResult = processNextStar(tempString, useful);
	}
	else if (relationship == "affects") {
		tempResult = processAffects(tempString, useful);
	}
	else if (relationship == "affects*") {
		tempResult = processAffectsStar(tempString, useful);
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
	else if (arrType == "stmtLst") {
		return PKB::getPKBInstance()->getStmtLstList();
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
			for (vector<vector<int>>::iterator t = r2.getContent().begin(); t != r2.getContent().end(); ++t) {
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
		updateMidResult(r2);
	}
	else if (newResult.getHeader().size() == 1){
		updateMidResult(newResult);
	}
	else {
		//noVariable
		return;
	}
}

void QueryEvaluator::updateMidResult(ResultTable newResult) {
	for (vector<ResultTable>::iterator it = midResult.begin(); it != midResult.end(); ++it) {
		if ((*it).getHeader().at(0) == newResult.getHeader().at(0)) {
			//merge these two 
			bool findIt;
			int count = 0;
			vector<vector<int>> content = (*it).getContent();
			
			for (vector<vector<int>>::iterator i = content.begin(); i != content.end(); ++i) {
				findIt = false;
				for (vector<vector<int>>::iterator t = newResult.getContent().begin(); t != newResult.getContent().end(); ++t) {
					if ((*i).at(0) == (*t).at(0)) {
						//find it 
						findIt = true;
						count++;
					}
				}
				if (!findIt) {
					(*it).eraseContent(count);
				}
				
			}
			return;
		}
	}
	// it is not inside intermediate result;
	midResult.push_back(newResult);
	return;
}

ResultTable QueryEvaluator::processModifies(Clause tempString, bool useful) {

	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	
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
			list<int> targetList = getList(arg1, arg1Type);
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = modifiesLine.begin(); i != modifiesLine.end(); i++) {
				if (isInList(targetList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
			
	}

	else if( arg2Type == "variable") {
		
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
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			//procedue, while, assign, if, call, stmt, prog_line
			list<int> targetList = getList(arg1, arg1Type);
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> varList = PKB::getPKBInstance()->getModifiesSecond(*i);
				for (list<int>::iterator j = varList.begin(); j != varList.end(); j++) {
					temp.push_back(*i);
					temp.push_back(*j);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else if (arg2Type == "all") {

		if (arg1Type == "number") {
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			list<int> varList = PKB::getPKBInstance()->getModifiesSecond(stoi(arg1));
			ResultTable tempResult = ResultTable();
			if (varList.empty()) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
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
			if (varList.empty()) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}
		else {
			//procedue, while, assign, if, call, stmt, prog_line
			list<int> targetList = getList(arg1, arg1Type);
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> varList = PKB::getPKBInstance()->getModifiesSecond(*i);
				if (!varList.empty()) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		SPALog::log("Error: Modifies arg2 wrong type");
		return ResultTable();
	}
	
}

ResultTable QueryEvaluator::processUses(Clause tempString, bool useful) {

	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	
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
			list<int> targetList = getList(arg1,arg1Type);
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = usesLine.begin(); i != usesLine.end(); i++) {
				if (isInList(targetList, *i)) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

	}

	else if (arg2Type == "variable") {
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
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {

			list<int> targetList = getList(arg1, arg1Type);
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> varList = PKB::getPKBInstance()->getUsesSecond(*i);
				for (list<int>::iterator j = varList.begin(); j != varList.end(); j++) {
					temp.push_back(*i);
					temp.push_back(*j);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else if (arg2Type == "all") {
		if (arg1Type == "number") {
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg1 is not a valid prog_line!\n");
				return tempResult;
			}
			list<int> varList = PKB::getPKBInstance()->getUsesSecond(stoi(arg1));
			ResultTable tempResult = ResultTable();
			if (varList.empty()) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
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
			ResultTable tempResult = ResultTable();
			if (varList.empty()) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}
		else {

			list<int> targetList = getList(arg1, arg1Type);
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> varList = PKB::getPKBInstance()->getUsesSecond(*i);
				if (!varList.empty()) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		SPALog::log("Error: Uses arg2 wrong type");
		return ResultTable();
	}

}

ResultTable QueryEvaluator::processParent(Clause tempString, bool useful) {
	
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	
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
		else if(arg2Type == "all"){
			ResultTable tempResult = ResultTable();
			if (childList.empty()) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}
		else {
			//stmt, assign, while, call, if, prog_line
			vector<int> temp;
			list<int> targetList = getList(arg2, arg2Type);
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
	else if(arg1Type == "all"){
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Parent arg2 is not a valid prog_line");
				return tempResult;
			}
			int parent = PKB::getPKBInstance()->getParentFirst(stoi(arg2));
			
			if (parent != 0) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg1List = getList(arg1, arg1Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = PKB::getPKBInstance()->getParentSecond(*t);
				if (!childList.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2Type can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				int parent = PKB::getPKBInstance()->getParentFirst(*t);
				if (parent != 0) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
	else {
		// arg1Type can be while, if, stmt, prog_line
		list<int> arg1List = getList(arg1, arg1Type);
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
				if (!useful) {
					return tempResult;
				}
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = PKB::getPKBInstance()->getParentSecond(*t);
				if (!childList.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			//arg2Type can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2,arg2Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
}

ResultTable QueryEvaluator::processFollows(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	
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
		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			//littlebro must exist
			tempResult.setIsWholeTrue(1);
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt, prog_line
			list<int> targetList = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			if (isInList(targetList, littleBrother)) {
				temp.push_back(littleBrother);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

	}
	else if (arg1Type == "all"){
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Follows arg2 is not a valid prog_line");
				return tempResult;
			}
			int brother = PKB::getPKBInstance()->getFollowsFirst(stoi(arg2));
			if (brother != 0) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				int brother = PKB::getPKBInstance()->getFollowsFirst(*t);
				if (brother!=0) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				int brother= PKB::getPKBInstance()->getFollowsFirst(*t);
				if (brother != 0) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
	else {
		// arg1 can be while, assign, call, stmt, if, prog_line
		list<int> arg1List = getList(arg1, arg1Type);
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
				if (!useful) {
					return tempResult;
				}
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				int littleBrother = PKB::getPKBInstance()->getFollowsSecond(*t);
				if (littleBrother!= 0) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2,arg2Type);
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
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

}

ResultTable QueryEvaluator::processParentStar(Clause tempString, bool useful) {

	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	
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

		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable();
			if (childList.empty()) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}

		else {
			//stmt, assign, while, call, if, prog_line
			vector<int> temp;
			list<int> targetList = getList(arg2, arg2Type);
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = childList.begin(); t != childList.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else if (arg1Type == "all") {
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Parent arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> parents = PKB::getPKBInstance()->getParentStarFirst(stoi(arg2));

			if (!parents.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg1List = getList(arg1, arg1Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = PKB::getPKBInstance()->getParentStarSecond(*t);
				if (!childList.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2Type can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				list<int> parents = PKB::getPKBInstance()->getParentStarFirst(*t);
				if (!parents.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		// arg1Type can be while, if, stmt, prog_line, all
		list<int> arg1List = getList(arg1, arg1Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> childList = PKB::getPKBInstance()->getParentStarSecond(*t);
				if (!childList.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else {
			//arg2Type can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
}

ResultTable QueryEvaluator::processFollowsStar(Clause tempString, bool useful) {

	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

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
		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			//littlebro must exist
			tempResult.setIsWholeTrue(1);
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt,prog_line
			list<int> targetList = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = littleBrothers.begin(); t != littleBrothers.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

	}

	else if (arg1Type == "all") {
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Follows arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> brother = PKB::getPKBInstance()->getFollowsStarFirst(stoi(arg2));
			if (!brother.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				list<int> brothers = PKB::getPKBInstance()->getFollowsStarFirst(*t);
				if (!brothers.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				list<int> brothers = PKB::getPKBInstance()->getFollowsStarFirst(*t);
				if (!brothers.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		// arg1 can be while, assign, call, stmt, if,prog_line, all
		list<int> arg1List = getList(arg1, arg1Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getFollowsStarSecond(*t);
				if (!littleBrothers.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else {
			//arg2 can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2, arg2Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
				
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

}

ResultTable QueryEvaluator::processCalls(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
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

		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable();
			if (!procedureCalled.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}

		else if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = procedureCalled.begin(); i != procedureCalled.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else if (arg1Type == "procedure" ) {
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
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsSecond(*i);
				if (!procedureCalled.empty()) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1,arg2);
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Calls invalid arg1 == arg2!\n");
				return tempResult;
			}

			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsSecond(*i);
				for (list<int>::iterator t = procedureCalled.begin(); t != procedureCalled.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else if (arg1Type == "all") {
		if (arg2Type == "string") {
			ResultTable tempResult = ResultTable();
			vector<int> temp;
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg2 is not a valid proc id!\n");
				return tempResult;
			}

			list<int> procedureCalls = PKB::getPKBInstance()->getCallsFirst(arg2ID);

			if (!procedureCalls.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}

			return tempResult;
		}
		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable();
			vector<int> temp;
			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsSecond(*i);
				if (!procedureCalled.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			
			return tempResult;
		}
		else if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;

			list<int> procList = getList(arg2, arg2Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalls = PKB::getPKBInstance()->getCallsFirst(*i);
				if (!procedureCalls.empty()) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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

ResultTable QueryEvaluator::processCallsStar(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
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
		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable();
			if (!procedureCalled.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			for (list<int>::iterator i = procedureCalled.begin(); i != procedureCalled.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else if (arg1Type == "procedure") {
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
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsStarSecond(*i);
				if (!procedureCalled.empty()) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Calls* invalid arg1 == arg2!\n");
				return tempResult;
			}

			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsStarSecond(*i);
				for (list<int>::iterator t = procedureCalled.begin(); t != procedureCalled.end(); t++) {
					temp.push_back(*i);
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			SPALog::log("arg2 is not valid!\n");
			return ResultTable();
		}
	}
	else if (arg1Type == "all") {
		if (arg2Type == "string") {
			ResultTable tempResult = ResultTable();
			vector<int> temp;
			int arg2ID = PKB::getPKBInstance()->getProcID(arg2);

			if (!arg2ID) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("arg2 is not a valid proc id!\n");
				return tempResult;
			}

			list<int> procedureCalls = PKB::getPKBInstance()->getCallsStarFirst(arg2ID);

			if (!procedureCalls.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}

			return tempResult;
		}
		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable();
			vector<int> temp;
			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalled = PKB::getPKBInstance()->getCallsStarSecond(*i);
				if (!procedureCalled.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}

			return tempResult;
		}
		else if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;

			list<int> procList = getList(arg2, arg2Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				list<int> procedureCalls = PKB::getPKBInstance()->getCallsStarFirst(*i);
				if (!procedureCalls.empty()) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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

ResultTable QueryEvaluator::processNext(Clause tempString, bool useful){
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

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
		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable();
			if (!littleBrothers.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, while, if, call, stmt, prog_line
			list<int> targetList = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = littleBrothers.begin(); t != littleBrothers.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

	}

	else if(arg1Type == "all"){
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Next arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> brothers = PKB::getPKBInstance()->getNextFirst(stoi(arg2));
			vector<int> temp;
			if (brothers.size() == 0) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg1List = getList(arg1, arg1Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getNextSecond(*t);
				if (!littleBrothers.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				list<int> brothers = PKB::getPKBInstance()->getNextFirst(*t);
				if (!brothers.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		// arg1 can be while, assign, call, stmt, if, prog_line
		list<int> arg1List = getList(arg1, arg1Type);
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
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getNextSecond(*t);
				if (!littleBrothers.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
				
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
}

ResultTable QueryEvaluator::processNextStar(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

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

		else if (arg2Type == "all") {
			ResultTable tempResult = ResultTable();
			if (!littleBrothers.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}

		else {
			// arg2 can be assign, while, if, call, stmt, prog_line
			list<int> targetList = getList(arg2,arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = littleBrothers.begin(); t != littleBrothers.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

	}

	else if (arg1Type == "all") {
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Next* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> brothers = PKB::getPKBInstance()->getNextStarFirst(stoi(arg2));
			vector<int> temp;
			if (brothers.size() == 0) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg1List = getList(arg1, arg1Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getNextStarSecond(*t);
				if (!littleBrothers.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, while, if, call, stmt, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				list<int> brothers = PKB::getPKBInstance()->getNextStarFirst(*t);
				if (!brothers.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		// arg1 can be while, assign, call, stmt, if, prog_line
		list<int> arg1List = getList(arg1, arg1Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getNextStarSecond(*t);
				if (!littleBrothers.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else {
			//arg2 can be assign, while, if, call, stmt, prog_line, all
			list<int> arg2List = getList(arg2,arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> littleBrothers = PKB::getPKBInstance()->getNextStarSecond(*t);
				for (list<int>::iterator i = littleBrothers.begin(); i != littleBrothers.end(); i++) {
					if (isInList(arg2List, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
}

ResultTable QueryEvaluator::processAffects(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

	if (arg1Type == "number") {
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Affects arg1 is not a valid prog_line");
			return tempResult;
		}

		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Affects arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isAffectsValid(stoi(arg1), stoi(arg2))) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> affected = PKB::getPKBInstance()->getAffectsSecond(stoi(arg1));
			
			ResultTable tempResult = ResultTable();
			if (!affected.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}
		else {
			// arg2 can be assign, prog_line
			list<int> affected = PKB::getPKBInstance()->getAffectsSecond(stoi(arg1));
			list<int> targetList = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = affected.begin(); t != affected.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

	}

	else if (arg1Type == "all") {
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Affects arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> affects = PKB::getPKBInstance()->getAffectsFirst(stoi(arg2));
			vector<int> temp;
			if (affects.size() == 0) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg1List = getList(arg1, arg1Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> affected = PKB::getPKBInstance()->getAffectsSecond(*t);
				if (!affected.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				list<int> affects = PKB::getPKBInstance()->getAffectsFirst(*t);
				if (!affects.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		// arg1 can be assign, prog_line
		list<int> arg1List = getList(arg1, arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Affects arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> affects = PKB::getPKBInstance()->getAffectsFirst(stoi(arg2));
			vector<int> temp;
			if (affects.size() == 0) {
				ResultTable tempResult = ResultTable();
				tempResult.setIsWholeTrue(0);
				return tempResult;
			}
			for (list<int>::iterator t = affects.begin(); t != affects.end(); t++) {
				if (isInList(arg1List, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> affected = PKB::getPKBInstance()->getAffectsSecond(*t);
				if (!affected.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> affected = PKB::getPKBInstance()->getAffectsSecond(*t);
				for (list<int>::iterator i = affected.begin(); i != affected.end(); i++) {
					if (isInList(arg2List, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}

			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
}

ResultTable QueryEvaluator::processAffectsStar(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

	if (arg1Type == "number") {
		if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg1))) {
			ResultTable tempResult = ResultTable();
			tempResult.setIsWholeTrue(0);
			SPALog::log("Affects* arg1 is not a valid prog_line");
			return tempResult;
		}

		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Affects* arg2 is not a valid prog_line");
				return tempResult;
			}
			if (PKB::getPKBInstance()->isAffectsStarValid(stoi(arg1), stoi(arg2))) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}

		else if (arg2Type == "all") {
			list<int> affected = PKB::getPKBInstance()->getAffectsStarSecond(stoi(arg1));
			ResultTable tempResult = ResultTable();
			if (!affected.empty()) {
				tempResult.setIsWholeTrue(1);
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			return tempResult;
		}

		else {
			// arg2 can be assign, prog_line
			list<int> targetList = getList(arg2, arg2Type);
			list<int> affected = PKB::getPKBInstance()->getAffectsStarSecond(stoi(arg1));
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = affected.begin(); t != affected.end(); t++) {
				if (isInList(targetList, *t)) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

	}

	else if (arg1Type == "all") {
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable();
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Affects* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> affects = PKB::getPKBInstance()->getAffectsStarFirst(stoi(arg2));
			vector<int> temp;
			if (affects.size() == 0) {
				tempResult.setIsWholeTrue(0);
			}
			else {
				tempResult.setIsWholeTrue(1);
			}
			return tempResult;
		}
		else if (arg2Type == "all") {
			list<int> arg1List = getList(arg1, arg1Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable();
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> affects = PKB::getPKBInstance()->getAffectsStarSecond(*t);
				if (!affects.empty()) {
					tempResult.setIsWholeTrue(1);
					break;
				}
			}
			return tempResult;
		}
		else {
			//arg2 can be assign, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg2);
			for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
				list<int> affects = PKB::getPKBInstance()->getAffectsStarFirst(*t);
				if (!affects.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}

	else {
		// arg1 can be assign, prog_line
		list<int> arg1List = getList(arg1, arg1Type);
		if (arg2Type == "number") {
			ResultTable tempResult = ResultTable(arg1);
			if (!PKB::getPKBInstance()->isValidStmtNo(stoi(arg2))) {
				tempResult.setIsWholeTrue(0);
				SPALog::log("Affects* arg2 is not a valid prog_line");
				return tempResult;
			}
			list<int> affects = PKB::getPKBInstance()->getAffectsStarFirst(stoi(arg2));
			vector<int> temp;
			if (affects.size() != 0) {
				for (list<int>::iterator t = affects.begin(); t != affects.end(); t++) {
					if (isInList(arg1List, *t)) {
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			else {
				tempResult.setIsWholeTrue(0);
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else if (arg2Type == "all") {
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> affected = PKB::getPKBInstance()->getAffectsStarSecond(*t);
				if (!affected.empty()) {
					temp.push_back(*t);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}

		else {
			//arg2 can be assign, prog_line
			list<int> arg2List = getList(arg2, arg2Type);
			vector<int> temp;
			ResultTable tempResult = ResultTable(arg1, arg2);
			for (list<int>::iterator t = arg1List.begin(); t != arg1List.end(); t++) {
				list<int> affected = PKB::getPKBInstance()->getAffectsStarSecond(*t);
				for (list<int>::iterator i = affected.begin(); i != affected.end(); i++) {
					if (isInList(arg2List, *i)) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
}

bool QueryEvaluator::processPatternClause(Clause tempString, bool useful) {
	
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string syn;
	string synType;
	if (tempString.getVarType().size() == 4) {
		syn = tempString.getVar().at(3);
		synType = tempString.getVarType().at(3);
	}
	else {
		syn = tempString.getVar().at(2);
		synType = tempString.getVarType().at(2);
	}
	
	
	string log = "Pattern clause: " + synType + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (synType == "assign") {
		tempResult = processAssignPattern(tempString, useful);
	}
	else if (synType == "while") {
		tempResult = processWhilePattern(tempString, useful);
	}
	else if (synType == "if") {
		tempResult = processIfPattern(tempString, useful);
	}
	else {
		SPALog::log("Wrong pattern!");
		return false;
	}

	if (isResultEmpty(tempResult)) {
		return false;
	}

	if (useful) {
		resultList.push_back(tempResult);
	}
	
	return true;
	
}

ResultTable QueryEvaluator::processAssignPattern(Clause tempString, bool useful) {
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
				if (!useful) {
					return tempResult;
				}
			}
		}
		else if (arg2Type == "string") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithBothExact(arg1, arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
		}
		else if (arg2Type == "substring") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithBoth(arg1, arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
		}
		else {
			SPALog::log("Pattern arg2 wrong type");
		}

		if (useful) {
			_updateMidResult(tempResult);
		}
		
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
					if (!useful) {
						return tempResult;
					}
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
					if (!useful) {
						return tempResult;
					}
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
					if (!useful) {
						return tempResult;
					}
				}
			}
		}
		else {
			SPALog::log("Pattern arg2 wrong type");
		}
		if (useful) {
			_updateMidResult(tempResult);
		}
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
				if (!useful) {
					return tempResult;
				}
			}
		}
		else if (arg2Type == "string") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecondExact(arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
		}
		else if (arg2Type == "substring") {
			list<int> assignList = PKB::getPKBInstance()->getAssignWithSecond(arg2);
			for (list<int>::iterator i = assignList.begin(); i != assignList.end(); i++) {
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
		}
		else {
			SPALog::log("Pattern arg2 wrong type");
		}
		if (useful) {
			_updateMidResult(tempResult);
		}
		return tempResult;
	}
	else {
		SPALog::log("pattern arg1 type wrong");
		return ResultTable();
	}
}

ResultTable QueryEvaluator::processWhilePattern(Clause tempString, bool useful) {
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
			if (!useful) {
				return tempResult;
			}
		}
		if (useful) {
			_updateMidResult(tempResult);
		}
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
				if (!useful) {
					return tempResult;
				}
			}
		}
		if (useful) {
			_updateMidResult(tempResult);
		}
		
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
			if (!useful) {
				return tempResult;
			}
		}
		if (useful) {
			_updateMidResult(tempResult);
		}
		
		return tempResult;
	}
}

ResultTable QueryEvaluator::processIfPattern(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	string syn = tempString.getVar().at(3);
	string synType = tempString.getVarType().at(3);

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
			if (useful) {
				return tempResult;
			}
		}
		if (!useful) {
			_updateMidResult(tempResult);
		}
		
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
				if (!useful) {
					return tempResult;
				}
			}
		}
		if (useful) {
			_updateMidResult(tempResult);
		}
		
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
			if (!useful) {
				return tempResult;
			}
		}
		if (useful) {
			_updateMidResult(tempResult);
		}
		
		return tempResult;
	}
}

bool QueryEvaluator::processSelectClause(Clause tempString, bool useful) {
	int tupleSize = tempString.getVar().size();
	for (int i = 0; i < tupleSize; i++) {

		string syn = tempString.getVar().at(i);
		string synType = tempString.getVarType().at(i);
		string log = "Select clause: select " + syn + ":" + synType + "\n";
		SPALog::log(log);

		if (synType == "variable") {
			ResultTable tempResult = ResultTable(syn);
			vector<int> temp;
			list<int> varTable = getList(syn,synType);
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
			list<int> procList = getList(syn, synType);
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
			list<int> targetList = getList(syn, synType);
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

bool QueryEvaluator::processWithClause(Clause tempString, bool useful) {
	string synType = tempString.getRelationship();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

	string log = "With clause: " + synType + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (synType == "withNumber") {
		tempResult = processNumberWith(tempString, useful);
	}
	else if (synType == "withName") {
		tempResult = processNameWith(tempString, useful);
	}
	else {
		SPALog::log("Wrong with type!");
		return false;
	}


	if (tempResult.getIsWholeTrue() == 1) {
		return true;
	}
	if (isResultEmpty(tempResult)) {
		return false;
	}

	if (useful) {
		resultList.push_back(tempResult);
	}
	
	
	return true;
}

bool QueryEvaluator::processWithConstClause(Clause tempString, bool useful) {
	string synType = tempString.getRelationship();
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);

	string log = "With constant clause: " + synType + "( " + arg1 + ":" + arg1Type + ", " + arg2 + ":" + arg2Type + ")\n";
	SPALog::log(log);

	ResultTable tempResult;

	if (synType == "withNumber") {
		tempResult = processNumberWith(tempString, useful);
	}
	else if (synType == "withName") {
		tempResult = processNameWith(tempString, useful);
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

ResultTable QueryEvaluator::processNameWith(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
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
			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				temp.push_back(*i);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList(arg2, arg2Type);
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				//get call stmt's procedure id
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				temp.push_back(procID);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			
			return tempResult;
		}
		else if (arg2Type == "variable") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> procList = getList(arg1, arg1Type);
			list<int> varList = getList(arg2,arg2Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					string varName = PKB::getPKBInstance()->getVarName(*t);
					if (procName == varName) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> procList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				if (procName == arg2) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			SPALog::log("Error: with arg2 wrong type");
			return ResultTable();
		}
	}
	else if (arg1Type == "call") {
		if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList(arg1, arg1Type);
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				//get call stmt's procedure id
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				temp.push_back(*i);
				temp.push_back(procID);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
			list<int> callList = getList(arg1, arg1Type);
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int proc1 = PKB::getPKBInstance()->getCallStmtProc(*i);
				for (list<int>::iterator t = callList.begin(); t != callList.end(); t++) {
					int proc2 = PKB::getPKBInstance()->getCallStmtProc(*t);
					if (proc1 == proc2) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
					
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "variable") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList(arg1, arg1Type);
			list<int> varList = getList(arg2,arg2Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}

			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> callList = getList(arg1, arg1Type);
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				string procName = PKB::getPKBInstance()->getProcName(procID);
				if (procName == arg2) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			SPALog::log("Error: with arg2 wrong type");
			return ResultTable();
		}
	}
	else if (arg1Type == "variable") {
		if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> procList = getList(arg2, arg2Type);
			list<int> varList = getList(arg1, arg1Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				for (list<int>::iterator t = varList.begin(); t != varList.end(); t++) {
					string varName = PKB::getPKBInstance()->getVarName(*t);
					if (procName == varName) {
						temp.push_back(*t);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> callList = getList(arg2, arg2Type);
			list<int> varList = getList(arg1, arg1Type);
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
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
			list<int> varList = getList(arg1, arg1Type);
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				temp.push_back(*i);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "string") {
			ResultTable tempResult = ResultTable(arg1);
			vector<int> temp;
			list<int> varList = getList(arg1,arg1Type);
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				string varName = PKB::getPKBInstance()->getVarName(*i);
				if (varName == arg2) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			SPALog::log("Error: with arg2 wrong type");
			return ResultTable();
		}
	}
	else if (arg1Type == "string") {
		if (arg2Type == "procedure") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			list<int> procList = getList(arg2, arg2Type);
			for (list<int>::iterator i = procList.begin(); i != procList.end(); i++) {
				string procName = PKB::getPKBInstance()->getProcName(*i);
				if (procName == arg1) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "call") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			list<int> callList = getList(arg2, arg2Type);
			for (list<int>::iterator i = callList.begin(); i != callList.end(); i++) {
				int procID = PKB::getPKBInstance()->getCallStmtProc(*i);
				string procName = PKB::getPKBInstance()->getProcName(procID);
				if (procName == arg1) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else if (arg2Type == "variable") {
			ResultTable tempResult = ResultTable(arg2);
			vector<int> temp;
			list<int> varList = getList(arg2, arg2Type);
			for (list<int>::iterator i = varList.begin(); i != varList.end(); i++) {
				string varName = PKB::getPKBInstance()->getVarName(*i);
				if (varName == arg1) {
					temp.push_back(*i);
					tempResult.addTuple(temp);
					temp.clear();
					if (!useful) {
						return tempResult;
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
			SPALog::log("Error: with arg2 wrong type");
			return ResultTable();
		}
	}
	else {
		SPALog::log("Error: with arg1 wrong type");
		return ResultTable();
	}

}

ResultTable QueryEvaluator::processNumberWith(Clause tempString, bool useful) {
	string arg1 = tempString.getVar().at(0);
	string arg1Type = tempString.getVarType().at(0);
	string arg2 = tempString.getVar().at(1);
	string arg2Type = tempString.getVarType().at(1);
	//arg1 and arg2 can be: prog_line, number, constant, (stmt, call, while, assign, call)
	if (arg1Type == "prog_line") {
		if (arg2Type == "prog_line") {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			if (arg1 == arg2) {
				tempResult.setIsWholeTrue(1);
				return tempResult;
			}
			list<int> stmtList = getList(arg1,arg1Type);
			for (list<int>::iterator i = stmtList.begin(); i != stmtList.end(); i++) {
				temp.push_back(*i);
				temp.push_back(*i);
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
		else {
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
			list<int> targetList = getList(arg1, arg1Type);
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				list<int> arg2List = getList(arg2, arg2Type);
				for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
					if (*i == *t) {
						temp.push_back(*i);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
			if (useful) {
				_updateMidResult(tempResult);
			}
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
			
			list<int> targetList = getList(arg2, arg2Type);
		
			if (isInList(targetList, stoi(arg1))) {
				temp.push_back(stoi(arg1));
				tempResult.addTuple(temp);
				temp.clear();
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
	else {
		list<int> targetList = getList(arg1, arg1Type);
		//arg1 == stmt, call, while, assign, constant
		if (arg2Type == "prog_line") {
			list<int> arg2List = getList(arg2, arg2Type);
			ResultTable tempResult = ResultTable(arg1, arg2);
			vector<int> temp;
		
			for (list<int>::iterator i = targetList.begin(); i != targetList.end(); i++) {
				for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
					if (*i == *t) {
						temp.push_back(*i);
						temp.push_back(*t);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
				if (!useful) {
					return tempResult;
				}
			}
			if (useful) {
				_updateMidResult(tempResult);
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
				list<int> arg2List = getList(arg2, arg2Type);
				for (list<int>::iterator t = arg2List.begin(); t != arg2List.end(); t++) {
					if (*i == *t) {
						temp.push_back(*i);
						temp.push_back(*i);
						tempResult.addTuple(temp);
						temp.clear();
						if (!useful) {
							return tempResult;
						}
					}
				}
				
			}
			if (useful) {
				_updateMidResult(tempResult);
			}
			return tempResult;
		}
	}
}

