#include "AST.h"
#include "Modifies.h"
#include "Uses.h"
#include "Follows.h"
#include "Parent.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "Pattern.h"
#include <string>
#include <list>

using namespace std;

#ifndef PKB_H
#define PKB_H

class PKB
{
public:
	//static PKB* getPKBInstance();
	PKB() {};
	ProcTable& getProcTable(void);
	VarTable& getVarTable(void);
	Modifies& getModifies(void);
	Uses& getUses(void);
	Parent& getParent(void);
	Follows& getFollows(void);
	AST& getAST(void);
	Pattern& getPattern();
	void addWhileList(int);
	void addAssignList(int);
	list<int> getWhileList(void);
	list<int> getAssignList(void);
	void setAST(AST);

private:
	//static PKB* _PKBInstance;
	Modifies modifies;
	Uses uses;
	ProcTable procTable;
	VarTable varTable;
	Follows follows;
	Parent parent;
	Pattern pattern;
	AST ast;
	list<int> whileList;
	list<int> assignList;
};

#endif