#include "PKB.h"
#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "VarTable.h"
#include <string>
#include <list>

using namespace std;

PKB::PKB()
{

}

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

Parent& PKB::getParent(void) {
	return parent;
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
