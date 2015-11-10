#include "Modifies.h"
#include "Uses.h"
#include "Follows.h"
#include "FollowsStar.h"
#include "Parent.h"
#include "ParentStar.h"
#include "Calls.h"
#include "CallsStar.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "Pattern.h"
#include "CFG.h"
#include "Statement.h"
#include "StatementType.h"
#include "SPALog.h"
#include <assert.h>
#include<sstream>

using namespace std;


#ifndef PKB_H
#define PKB_H

typedef int INDEX;
typedef int VARIABID;
typedef int PROCID;
typedef int STMTLINE;
typedef string VARNAME;
typedef string PROCNAME;

class PKB
{
public:
	static PKB* getPKBInstance();
	static void DestroyInstance();
	
	//general
	bool isValidStmtNo(int);
	void addStmtToList(int, StatementType);
	void addConstantToList(int);
	void addToCallStmtProcMap(int, int);
	list<PROCID> getProcList();
	list<VARIABID> getVarList();
	list<STMTLINE> getWhileList();
	list<STMTLINE> getAssignList();
	list<STMTLINE> getCallList();
	list<STMTLINE> getIfList();
	list<STMTLINE> getStmtList(); //everything
	list<STMTLINE> getParentList(); //while and if list
	list<int> getConstantList();
	int getCallStmtProc(int);
	int getProcIDSpecial(int);
	int getFirstStatement(int);
	StatementType getType(int);
	int getStmtSize();
	void logWhileList();
	void logAssignList();
	void logCallList();
	void logIfList();
	void logConstantList();
	void logParentList();
	void logStmtList();
	void logCallStmtProcMap();
	void buildCFG(list<Statement>);
	//PKB housekeeping function
	//sort and unify function for Modifies, Uses, FollowsStar, Parent, ParentStar
	//unify ifList and constantList
	void houseKeeping();


	//varTable
	VARIABID insertVar(VARNAME);
	VARIABID getVarID(VARNAME); 	//return 0 for invalid varName input
	VARNAME getVarName(VARIABID);
	void logVarTable();
	void setVarTableReverse();


	//procTable
	PROCID insertProc(PROCNAME);
	PROCID getProcID(PROCNAME); 	//return 0 for invalid procName input
	PROCNAME getProcName(PROCID);
	void logProcTable();
	void setProcTableReverse();
	

	//Modifies
	void setModifies(INDEX, list<VARIABID>);
	void resetModifies(INDEX, list<VARIABID>);
	list<INDEX> getModifiesFirst(VARIABID);
	list<VARIABID> getModifiesSecond(INDEX);
	bool isModifiesValid(INDEX, VARIABID);
	void logModifies();
	void setModifiesReverse();


	//Uses
	void setUses(INDEX, list<VARIABID>);
	void resetUses(INDEX, list<VARIABID>);
	list<INDEX> getUsesFirst(VARIABID);
	list<VARIABID> getUsesSecond(INDEX);
	bool isUsesValid(INDEX, VARIABID);
	void logUses();
	void setUsesReverse();


	//Pattern
	void setPattern(int, string, string);
	list<int> getAssignWithFirstExact(string);         //pattern a ("x", _)
	list<int> getAssignWithSecond(string);             //pattern a (_, _"x"_)
	list<int> getAssignWithSecondExact(string);        //pattern a (_, "x")
	list<int> getAssignWithBoth(string, string);       //pattern a ("x", _"x"_)
	list<int> getAssignWithBothExact(string, string);  //pattern a ("x", "x")
	list<int> getIfWithFirstExact(string);
	list<int> getWhileWithFirstExact(string);
	void sortAndUnifyMap();
	void logPattern();


	//Follows
	void setFollows(STMTLINE, STMTLINE);
	STMTLINE getFollowsFirst(STMTLINE);
	STMTLINE getFollowsSecond(STMTLINE);
	bool isFollowsValid(STMTLINE, STMTLINE);
	void logFollows();
	map<STMTLINE, STMTLINE> getFollowsMap();
	void setFollowsReverse();


	//FollowsStar
	void setFollowsStar(STMTLINE, list<STMTLINE>);
	list<STMTLINE> getFollowsStarFirst(STMTLINE);
	list<STMTLINE> getFollowsStarSecond(STMTLINE);
	bool isFollowsStarValid(STMTLINE, STMTLINE);
	void logFollowsStar();
	void setFollowsStarReverse();


	//Calls
	void setCalls(PROCID, PROCID);
	list<PROCID> getCallsFirst(PROCID);
	list<PROCID> getCallsSecond(PROCID);
	bool isCallsValid(PROCID, PROCID);
	void logCalls();
	void sortAndUnifyCallsMap();
	map<PROCID, list<PROCID>> getCallsMap();
	void setCallsReverse();


	//CallsStar
	void setCallsStar(PROCID, list<PROCID>);
	list<PROCID> getCallsStarFirst(PROCID);
	list<PROCID> getCallsStarSecond(PROCID);
	bool isCallsStarValid(PROCID, PROCID);
	void logCallsStar();
	void sortAndUnifyCallsStarMap();
	map<PROCID, list<PROCID>> getCallsStarMap();
	void setCallsStarReverse();


	//Parent
	void setParent(STMTLINE, list<STMTLINE>);
	STMTLINE getParentFirst(STMTLINE);
	list<STMTLINE> getParentSecond(STMTLINE);
	bool isParentValid(STMTLINE, STMTLINE);
	void logParent();
	map<STMTLINE, list<STMTLINE>> getParentMap();
	void setParentReverse();


	//ParentStar
	void setParentStar(STMTLINE, list<STMTLINE>);
	list<STMTLINE> getParentStarFirst(STMTLINE);
	list<STMTLINE> getParentStarSecond(STMTLINE);
	bool isParentStarValid(STMTLINE, STMTLINE);
	void logParentStar();
	void setParentStarReverse();


	//Next
	list<STMTLINE> getNextFirst(STMTLINE);
	list<STMTLINE> getNextSecond(STMTLINE);
	bool isNextvalid(STMTLINE, STMTLINE);
	void logNext();

	//NextStar
	list<STMTLINE> getNextStarFirst(STMTLINE);
	list<STMTLINE> getNextStarSecond(STMTLINE);
	bool isNextStarValid(STMTLINE, STMTLINE);

	//Affect
	list<int> getAffectsFirst(int);
	list<int> getAffectsSecond(int);
	bool isAffectsValid(int, int);

	//AffectStar
	list<int> getAffectsStarFirst(int);
	list<int> getAffectsStarSecond(int);
	bool isAffectsStarValid(int, int);

private:
	static PKB* PKBInstance;
	PKB() {};
	Modifies modifies;
	Uses uses;
	ProcTable procTable;
	VarTable varTable;
	Follows follows;
	FollowsStar followsStar;
	ParentStar parentStar;
	Parent parent;
	Pattern pattern;
	Calls calls;
	CallsStar callsStar;
	CFG cfg;
	list<int> whileStmtList;
	list<int> assignStmtList;
	list<int> callStmtList;
	list<int> ifStmtList;
	list<int> constantList;
	map<int, int> callStmtProcMap;
	SPALog _log;

	ProcTable& getProcTable();
	VarTable& getVarTable();
	Modifies& getModifies();
	Uses& getUses();
	Parent& getParent();
	ParentStar& getParentStar();
	Follows& getFollows();
	FollowsStar& getFollowsStar();
	Calls& getCalls();
	CallsStar& getCallsStar();
	Pattern& getPattern();

	size_t getVarTableSize();
	size_t getProcTableSize();

	bool isAssignment(int);
	bool isWhile(int);
	bool isIf(int);
	bool isSameProc(int, int);
	bool contains(list<int>, int);
	void transfer(int, queue<int>&, bool);
	void clearQueue(queue<int>&);
	list<int> merge(list<int>&, list<int>&);
	list<int>::iterator search(list<int>::iterator,
		list<int>::iterator, int);
	list<int> insertAll(list<int>&, list<int>&);
};

#endif