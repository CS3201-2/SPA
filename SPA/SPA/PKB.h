#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "VarTable.h"
#include <string>
#include <list>

using namespace std;

#ifndef PKB_H
#define PKB_H

class PKB
{
public:
	PKB();
	ProcTable getProcTable(void);
	VarTable getVarTable(void);

private:
	AST AST;
	Modifies modifies;
	Uses uses;
	ProcTable procTable;
	VarTable varTable;
};

#endif