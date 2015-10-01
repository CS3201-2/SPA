#include "Modifies.h"
#include "Uses.h"
#include "Follows.h"
#include "FollowsStar.h"
#include "Parent.h"
#include "ParentStar.h"
#include "Calls.h"
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
	static PKB* getPKBInstance();
	//PKB() {};

	//general
	bool isValidStmtNo(int);
	void addWhileToList(int);
	void addAssignToList(int);
	void addCallToList(int);
	void addIfToList(int);
	list<int> getProcList();
	list<int> getVarList();
	list<int> getWhileList();
	list<int> getAssignList();
	list<int> getCallList();
	list<int> getIfList();
	list<int> getStmtList(); //everything
	list<int> getParentList(); //while and if list

	//PKB housekeeping function
	//sort and unify function for Modifies, Uses, FollowsStar, CallsStar, Parent, ParentStar
	//unify ifList
	void houseKeeping();

	//varTable
	int insertVar(string);
	int getVarID(string); 	//return 0 for invalid varName input
	string getVarName(int);
	void logVarTable();

	//procTable
	int insertProc(string);
	int getProcID(string); 	//return 0 for invalid procName input
	string getProcName(int);
	void logProcTable();
	
	//Modifies
	void setModifies(int, list<int>); // input parameter to be decided later
	list<int> getModifiesFirst(int);
	list<int> getModifiesSecond(int);
	bool isModifiesValid(int, int);
	void logModifies();

	//Uses
	void setUses(int, list<int>); //input parameter to be decided later
	list<int> getUsesFirst(int);
	list<int> getUsesSecond(int);
	bool isUsesValid(int, int);
	void logUses();

	//Pattern
	void setPattern(int, string, string);
	list<int> getAssignWithFirstExact(string);         //pattern a ("x", _)
	list<int> getAssignWithSecond(string);             //pattern a (_, _"x"_)
	list<int> getAssignWithSecondExact(string);        //pattern a (_, "x")
	list<int> getAssignWithBoth(string, string);       //pattern a ("x", _"x"_)
	list<int> getAssignWithBothExact(string, string);  //pattern a ("x", "x")
	list<int> getIfWithFirstExact(string);
	list<int> getWhileWithFirstExact(string);
	void logPattern();

	//Follows
	void setFollows(int, int);
	int getFollowsFirst(int);
	int getFollowsSecond(int);
	bool isFollowsValid(int, int);
	void logFollows();
	map<int, int> getFollowsMap();

	//FollowsStar
	void setFollowsStar(int, list<int>); 
	list<int> getFollowsStarFirst(int);
	list<int> getFollowsStarSecond(int);
	bool isFollowsStarValid(int, int);
	void logFollowsStar();

	//Calls
	void setCalls(int, int);
	int getCallsFirst(int);
	int getCallsSecond(int);
	bool isCallsValid(int, int);
	void logCalls();

	//CallsStar
	/*void setCallsStar(int, list<int>);
	list<int> getCallsStarFirst(int);
	list<int> getCallsStarSecond(int);
	bool isCallsStarValid(int, int);
	void printAllCallsStar();*/

	//Parent
	void setParent(int, list<int>); 
	int getParentFirst(int);
	list<int> getParentSecond(int);
	bool isParentValid(int, int);
	void logParent();
	map<int, list<int>> getParentMap();

	//ParentStar
	void setParentStar(int, list<int>);
	list<int> getParentStarFirst(int);
	list<int> getParentStarSecond(int);
	bool isParentStarValid(int, int);
	void logParentStar();

	//Next
	/*void setNext(int, int);
	list<int> getNextFirst(int);
	list<int> getNextSecond(int);
	bool isNextvalid(int, int);
	void printAllNext();*/
	//new APIs ends


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
	list<int> whileStmtList;
	list<int> assignStmtList;
	list<int> callStmtList;
	list<int> ifStmtList;

	ProcTable& getProcTable();
	VarTable& getVarTable();
	Modifies& getModifies();
	Uses& getUses();
	Parent& getParent();
	ParentStar& getParentStar();
	Follows& getFollows();
	FollowsStar& getFollowsStar();
	Calls& getCalls();
	Pattern& getPattern();

	size_t getVarTableSize();
	size_t getProcTableSize();
};

#endif