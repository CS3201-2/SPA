#include "PKB.h"

using namespace std;


PKB* PKB::PKBInstance = NULL;

PKB* PKB::getPKBInstance() {
	if (!PKBInstance) {
		PKBInstance = new PKB();
	}

	return PKBInstance;
}

ProcTable& PKB::getProcTable() {
	return procTable;
}

VarTable& PKB::getVarTable() {
	return varTable;
}

Modifies& PKB::getModifies() {
	return modifies;
}

Uses& PKB::getUses() {
	return uses;
}

Follows& PKB::getFollows() {
	return follows;
}

FollowsStar& PKB::getFollowsStar() {
	return followsStar;
}

Calls& PKB::getCalls() {
	return calls;
}

Parent& PKB::getParent() {
	return parent;
}

ParentStar& PKB::getParentStar() {
	return parentStar;
}

Pattern& PKB::getPattern() {
	return pattern;
}

void PKB::houseKeeping() {
	getModifies().sortAndUnifyMap();
	getUses().sortAndUnifyMap();
	//getParent().sortAndUnifyMap();
	//getFollowsStar().sortAndUnifyMap();
	//getParentStar().sortAndUnifyMap();
	ifStmtList.unique();
}

//general
bool PKB::isValidStmtNo(int stmtNo) {
	size_t totalNoStmt = assignStmtList.size() + whileStmtList.size() + ifStmtList.size() + callStmtList.size();
	return (stmtNo > 0 && (size_t)stmtNo <= totalNoStmt);
}

void PKB::addWhileToList(int whileStmt) {
	whileStmtList.push_back(whileStmt);
}

void PKB::addAssignToList(int assignStmt) {
	assignStmtList.push_back(assignStmt);
}

void PKB::addCallToList(int callStmt) {
	callStmtList.push_back(callStmt);
}
//Note: might insert duplicate ifStmt
void PKB::addIfToList(int ifStmt) {
	ifStmtList.push_back(ifStmt);
}

list<int> PKB::getProcList() {
	size_t totalNoProc = getProcTableSize();
	list<int> procList;
	for (int i = 1; i <= totalNoProc; ++i) {
		procList.push_back(0 - i);
	}

	return procList;
}

list<int> PKB::getVarList() {
	size_t totalNoVar = getVarTableSize();
	list<int> varList;
	for (int i = 1; i <= totalNoVar; ++i) {
		varList.push_back(i);
	}

	return varList;
}

list<int> PKB::getWhileList() {
	return whileStmtList;
}

list<int> PKB::getAssignList() {
	return assignStmtList;
}

list<int> PKB::getCallList() {
	return callStmtList;
}

list<int> PKB::getIfList() {
	return ifStmtList;
}

list<int> PKB::getStmtList() {
	size_t totalNoStmt = assignStmtList.size() + whileStmtList.size() + ifStmtList.size() + callStmtList.size();
	list<int> stmtList;
	for (int i = 1; i <= totalNoStmt; ++i) {
		stmtList.push_back(i);
	}

	return stmtList;
} 

list<int> PKB::getParentList() {
	list<int> parentList = whileStmtList;
	parentList.insert(parentList.end(), whileStmtList.begin(), whileStmtList.end());
	parentList.sort();
	
	return parentList;
}


//varTable
int PKB::insertVar(string varName) {
	return getVarTable().insertVar(varName);
}

int PKB::getVarID(string varName) {
	return getVarTable().getVarID(varName);
}

string PKB::getVarName(int varID) {
	return getVarTable().getVarName(varID);
}

void PKB::logVarTable() {
	getVarTable().logVarTable();
}

size_t PKB::getVarTableSize() {
	return getVarTable().getVarTableSize();
}


//procTable
int PKB::insertProc(string procName) {
	return getProcTable().insertProc(procName);
}

int PKB::getProcID(string procName) {
	return getProcTable().getProcID(procName);
} 

string PKB::getProcName(int procID) {
	return getProcTable().getProcName(procID);
}

void PKB::logProcTable() {
	getProcTable().logProcTable();
}

size_t PKB::getProcTableSize() {
	return getProcTable().getProcTableSize();
}


//Modifies
void PKB::setModifies(int first, list<int> second) {
	getModifies().setModifies(first, second);
} // input parameter to be decided later

list<int> PKB::getModifiesFirst(int second) {
	return getModifies().getModifiesFirst(second);
}

list<int> PKB::getModifiesSecond(int first) {
	return getModifies().getModifiesSecond(first);
}

bool PKB::isModifiesValid(int first, int second) {
	return getModifies().isModifiesValid(first, second);
}

void PKB::logModifies() {
	getModifies().logModifies();
}


//Uses
void PKB::setUses(int first, list<int> second) {
	getUses().setUses(first, second);
} //input parameter to be decided later

list<int> PKB::getUsesFirst(int second) {
	return getUses().getUsesFirst(second);
}

list<int> PKB::getUsesSecond(int first) {
	return getUses().getUsesSecond(first);
}

bool PKB::isUsesValid(int first, int second) {
	return getUses().isUsesValid(first, second);
}

void PKB::logUses() {
	getUses().logUses();
}


//Pattern
void PKB::setPattern(int stmtNo, string first, string second) {
	getPattern().setPattern(stmtNo, first, second);
}

list<int> PKB::getAssignWithFirstExact(string first) {
	return getPattern().getAssignWithFirstExact(first);
}

list<int> PKB::getAssignWithSecondExact(string second) {
	return getPattern().getAssignWithSecondExact(second);
}

list<int> PKB::getAssignWithSecond(string second) {
	return getPattern().getAssignWithSecond(second);
}

list<int> PKB::getAssignWithBothExact(string first, string second) {
	return getPattern().getAssignWithBothExact(first, second);
}

list<int> PKB::getAssignWithBoth(string first, string second) {
	return getPattern().getAssignWithBoth(first, second);
}

list<int> PKB::getIfWithFirstExact(string first) {
	return getPattern().getIfWithFirstExact(first);
}
list<int> PKB::getWhileWithFirstExact(string first) {
	return getPattern().getWhileWithFisrtExact(first);
}

void PKB::logPattern() {
	getPattern().logPattern();
}


//Follows
void PKB::setFollows(int first, int second) {
	getFollows().setFollows(first, second);
}

int PKB::getFollowsFirst(int second) {
	return getFollows().getFollowsFirst(second);
}

int PKB::getFollowsSecond(int first) {
	return getFollows().getFollowsSecond(first);
}

bool PKB::isFollowsValid(int first, int second) {
	return getFollows().isFollowValid(first, second);
}

void PKB::logFollows() {
	getFollows().logFollows();
}

map<int, int> PKB::getFollowsMap() {
	return getFollows().getFollowsMap();
}


//FollowsStar
void PKB::setFollowsStar(int first, list<int> second) {
	getFollowsStar().setFollowsStar(first, second);
}

list<int> PKB::getFollowsStarFirst(int second) {
	return getFollowsStar().getFollowsStarFirst(second);
}

list<int> PKB::getFollowsStarSecond(int first) {
	return getFollowsStar().getFollowsStarSecond(first);
}

bool PKB::isFollowsStarValid(int first, int second) {
	return getFollowsStar().isFollowsStarValid(first, second);
}

void PKB::logFollowsStar() {
	getFollowsStar().logFollowsStar();
}


//Calls
void PKB::setCalls(int first, int second) {
	getCalls().setCalls(first, second);
}

list<int> PKB::getCallsFirst(int second) {
	return getCalls().getCallsFirst(second);
}

list<int> PKB::getCallsSecond(int first) {
	return getCalls().getCallsSecond(first);
}

bool PKB::isCallsValid(int first, int second) {
	return getCalls().isCallValid(first, second);
}

void PKB::logCalls() {
	getCalls().logCalls();
}

/*
//CallsStar
void setCallsStar(int, list<int>);
list<int> getCallsStarFirst(int);
list<int> getCallsStarSecond(int);
bool isCallsStarValid(int, int);
void printAllCallsStar();
*/


//Parent
void PKB::setParent(int first, list<int> second) {
	getParent().setParent(first, second);
}

int PKB::getParentFirst(int second) {
	return getParent().getParentFirst(second);
}

list<int> PKB::getParentSecond(int first) {
	return getParent().getParentSecond(first);
}

bool PKB::isParentValid(int first, int second) {
	return getParent().isParentValid(first, second);
}

void PKB::logParent() {
	getParent().logParent();
}

map<int, list<int>> PKB::getParentMap() {
	return getParent().getParentMap();
}


//ParentStar
void PKB::setParentStar(int first, list<int> second) {
	getParentStar().setParentStar(first, second);
}

list<int> PKB::getParentStarFirst(int second) {
	return getParentStar().getParentStarFirst(second);
}

list<int> PKB::getParentStarSecond(int first) {
	return getParentStar().getParentStarSecond(first);
}

bool PKB::isParentStarValid(int first, int second) {
	return getParentStar().isParentStarValid(first, second);
}

void PKB::logParentStar() {
	getParentStar().logParentStar();
}
/*
//Next
void setNext(int, int);
list<int> getNextFirst(int);
list<int> getNextSecond(int);
bool isNextvalid(int, int);
void printAllNext();
*/