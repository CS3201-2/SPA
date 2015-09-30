#include "PKB.h"
#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "VarTable.h"
#include <string>
#include <list>

using namespace std;

/*PKB* PKB::_PKBInstance = NULL;

PKB* PKB::getPKBInstance() {
	if (!_PKBInstance) {
		_PKBInstance = new PKB;
	}

	return _PKBInstance;
}*/

ProcTable& PKB::getProcTable(void) {
	return procTable;
}

VarTable& PKB::getVarTable(void) {
	return varTable;
}

Modifies& PKB::getModifies(void) {
	return modifies;
}

Uses& PKB::getUses(void) {
	return uses;
}

Follows& PKB::getFollows(void) {
	return follows;
}

Calls& PKB::getCalls() {
	return calls;
}

Parent& PKB::getParent(void) {
	return parent;
}

Pattern& PKB::getPattern() {
	return pattern;
}

AST& PKB::getAST(void) {
	return ast;
}

void PKB::addWhileList(int stmtLine) {
	if (find(whileList.begin(), whileList.end(), stmtLine) == whileList.end()) {
		whileList.push_back(stmtLine);
	}
}

void PKB::addAssignList(int stmtLine) {
	if (find(assignList.begin(), assignList.end(), stmtLine) == assignList.end()) {
		assignList.push_back(stmtLine);
	}
}

list<int> PKB::getWhileList(void) {
	return whileList;
}
list<int> PKB::getAssignList(void) {
	return assignList;
}

void PKB::setAST(AST a)
{
	ast = a;
}

//general
/*
bool isValidStmtNo(int);
void addWhileToList(int);
void addAssignToList(int);
void addCallToList(int);
void addIfToList(int);
list<int> getWhileList();
list<int> getAssignList();
list<int> getCallList();
list<int> getIfList();


//varTable
int insertVar(string);
int getVarID(string); 	//return 0 for invalid varName input
string getVarName(int);
void printVarTable();


//procTable
int insertProc(string);
int getProcID(string); 	//return 0 for invalid procName input
string getProcName(int);
void printProcTable();


//Modifies
void setModifies(); // input parameter to be decided later
list<int> getModifiesFirst(int);
list<int> getModifiesSecond(int);
bool isModifiesValid(int, int);
void printAllModifies();


//Uses
void setUses(); //input parameter to be decided later
list<int> getUsesFirst(int);
list<int> getUsesSecond(int);
bool isUsesValid(int, int);
void printAllUses();
*/


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

void PKB::printAllPattern() {
	getPattern().printAllPattern();
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

void PKB::printAllFollows() {
	getFollows().printFollowsMap();
}

/*
//FollowsStar
void setFollowsStar(int, list<int>);
list<int> getFollowsStarFirst(int);
list<int> getFollowsStarSecond(int);
bool isFollowsStarValid(int, int);
void printAllFollowsStar();
*/


//Calls
void PKB::setCalls(int first, int second) {
	getCalls().setCalls(first, second);
}

int PKB::getCallsFirst(int second) {
	getCalls().getCallsFirst(second);
}

int PKB::getCallsSecond(int first) {
	getCalls().getCallsSecond(first);
}

bool PKB::isCallsValid(int first, int second) {
	getCalls().isCallValid(first, second);
}

void PKB::printAllCalls() {
	getCalls().printAllCalls();
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

void PKB::printAllParent() {
	return getParent().printAllParent();
}

/*
//ParentStar
void setParentStar(int, list<int>);
list<int> getParentStarFirst(int);
list<int> getParentStarSecond(int);
bool isParentStarValid(int, int);
void printAllParent();

//Next
void setNext(int, int);
list<int> getNextFirst(int);
list<int> getNextSecond(int);
bool isNextvalid(int, int);
void printAllNext();
*/