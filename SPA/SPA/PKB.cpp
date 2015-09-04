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
