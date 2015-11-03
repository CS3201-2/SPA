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
	list<int> getProcList();
	list<int> getVarList();
	list<int> getWhileList();
	list<int> getAssignList();
	list<int> getCallList();
	list<int> getIfList();
	list<int> getStmtList(); //everything
	list<int> getStmtLstList();
	list<int> getParentList(); //while and if list
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
	void logStmtLstList();
	void logCallStmtProcMap();
	void buildCFG(list<Statement>);
	//PKB housekeeping function
	//sort and unify function for Modifies, Uses, FollowsStar, Parent, ParentStar
	//unify ifList and constantList
	void houseKeeping();


	//varTable
	int insertVar(string);
	int getVarID(string); 	//return 0 for invalid varName input
	string getVarName(int);
	void logVarTable();
	void setVarTableReverse();


	//procTable
	int insertProc(string);
	int getProcID(string); 	//return 0 for invalid procName input
	string getProcName(int);
	void logProcTable();
	void setProcTableReverse();
	

	//Modifies
	void setModifies(int, list<int>);
	void resetModifies(int, list<int>);
	list<int> getModifiesFirst(int);
	list<int> getModifiesSecond(int);
	bool isModifiesValid(int, int);
	void logModifies();
	void setModifiesReverse();


	//Uses
	void setUses(int, list<int>); 
	void resetUses(int, list<int>);
	list<int> getUsesFirst(int);
	list<int> getUsesSecond(int);
	bool isUsesValid(int, int);
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
	void setFollows(int, int);
	int getFollowsFirst(int);
	int getFollowsSecond(int);
	bool isFollowsValid(int, int);
	void logFollows();
	map<int, int> getFollowsMap();
	void setFollowsReverse();


	//FollowsStar
	void setFollowsStar(int, list<int>); 
	list<int> getFollowsStarFirst(int);
	list<int> getFollowsStarSecond(int);
	bool isFollowsStarValid(int, int);
	void logFollowsStar();
	void setFollowsStarReverse();


	//Calls
	void setCalls(int, int);
	list<int> getCallsFirst(int);
	list<int> getCallsSecond(int);
	bool isCallsValid(int, int);
	void logCalls();
	void sortAndUnifyCallsMap();
	map<int, list<int>> getCallsMap();
	void setCallsReverse();


	//CallsStar
	void setCallsStar(int, list<int>);
	list<int> getCallsStarFirst(int);
	list<int> getCallsStarSecond(int);
	bool isCallsStarValid(int, int);
	void logCallsStar();
	void sortAndUnifyCallsStarMap();
	map<int, list<int>> getCallsStarMap();
	void setCallsStarReverse();


	//Parent
	void setParent(int, list<int>); 
	int getParentFirst(int);
	list<int> getParentSecond(int);
	bool isParentValid(int, int);
	void logParent();
	map<int, list<int>> getParentMap();
	void setParentReverse();


	//ParentStar
	void setParentStar(int, list<int>);
	list<int> getParentStarFirst(int);
	list<int> getParentStarSecond(int);
	bool isParentStarValid(int, int);
	void logParentStar();
	void setParentStarReverse();


	//Next
	list<int> getNextFirst(int);
	list<int> getNextSecond(int);
	bool isNextvalid(int, int);
	void logNext();

	//NextStar
	list<int> getNextStarFirst(int);
	list<int> getNextStarSecond(int);
	bool isNextStarValid(int, int);

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
	list<int> stmtLstList;
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
};

#endif