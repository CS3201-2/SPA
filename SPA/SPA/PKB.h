#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "Follows.h"
#include "Parent.h"
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
	ProcTable& getProcTable(void);
	VarTable& getVarTable(void);
	Modifies& getModifies(void);
	Uses& getUses(void);
	Parent& getParent(void);
	Follows& getFollows(void);
	AST& getAST(void);
	void addWhileList(int);
	void addAssignList(int);
	list<int> getWhileList(void);
	list<int> getAssignList(void);
	void setAST(AST);

private:
	Modifies modifies;
	Uses uses;
	ProcTable procTable;
	VarTable varTable;
	Follows follows;
	Parent parent;
	AST ast;
	list<int> whileList;
	list<int> assignList;
};

#endif