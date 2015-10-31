#include "PKB.h"

using namespace std;


PKB* PKB::PKBInstance = NULL;

PKB* PKB::getPKBInstance() {
	if (!PKBInstance) {
		PKBInstance = new PKB();
	}

	return PKBInstance;
}

void PKB::DestroyInstance() {
	delete PKBInstance;
	PKBInstance = 0;
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

CallsStar& PKB::getCallsStar() {
	return callsStar;
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
	getParent().sortAndUnifyMap();
	//getFollowsStar().sortAndUnifyMap();
	//getParentStar().sortAndUnifyMap();
	getPattern().sortAndUnifyMap();
	ifStmtList.sort();
	ifStmtList.unique();
	constantList.sort();
	constantList.unique();
}

//general
bool PKB::isValidStmtNo(int stmtNo) {
	size_t totalNoStmt = assignStmtList.size() + whileStmtList.size() + ifStmtList.size() + callStmtList.size();
	return (stmtNo > 0 && (size_t)stmtNo <= totalNoStmt);
}

void PKB::addStmtToList(int stmtNo, StatementType stmtType) {
	switch (stmtType) {
	case assignmentStmt: assignStmtList.push_back(stmtNo); break;
	case procCallStmt: callStmtList.push_back(stmtNo); break;
	case whileStmt: whileStmtList.push_back(stmtNo); break;
	case ifStmt: ifStmtList.push_back(stmtNo); break;
	}
}

void PKB::addConstantToList(int constant) {
	constantList.push_back(constant);
}

void PKB::addToCallStmtProcMap(int stmtNo, int procID) {
	callStmtProcMap[stmtNo] = procID;
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
	parentList.insert(parentList.end(), ifStmtList.begin(), ifStmtList.end());
	parentList.sort();
	
	return parentList;
}

list<int> PKB::getConstantList() {
	return constantList;
}

int PKB::getCallStmtProc(int stmtNo) {
	if (callStmtProcMap.find(stmtNo) == callStmtProcMap.end()) {
		return 0;
	}
	else {
		return callStmtProcMap.at(stmtNo);
	}
}

int PKB::getProcIDSpecial(int i)
{
	return cfg.getProc(i);
}

int PKB::getFirstStatement(int i)
{
	return cfg.getFirstStatement(i);
}

void PKB::logWhileList() {
	string str = "while list\n";
	for (list<int>::iterator it = whileStmtList.begin(); it != whileStmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::log(str);
}

void PKB::logAssignList() {
	string str = "assign list\n";
	for (list<int>::iterator it = assignStmtList.begin(); it != assignStmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::log(str);
}

void PKB::logCallList() {
	string str = "call list\n";
	for (list<int>::iterator it = callStmtList.begin(); it != callStmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::log(str);
}

void PKB::logIfList() {
	string str = "if list\n";
	for (list<int>::iterator it = ifStmtList.begin(); it != ifStmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::log(str);
}

void PKB::logConstantList() {
	string str = "constant list\n";
	for (list<int>::iterator it = constantList.begin(); it != constantList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::log(str);
}

void PKB::logParentList() {
	string str = "parent list\n";
	list<int> parentList = getParentList();
	for (list<int>::iterator it = parentList.begin(); it != parentList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::log(str);
}

void PKB::logStmtList() {
	string str = "stmt list\n";
	list<int> stmtList = getStmtList();
	for (list<int>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::log(str);
}

void PKB::logCallStmtProcMap() {
	string str = "call stmt map\n";
	ProcTable pt = getProcTable();
	for (map<int, int>::iterator it = callStmtProcMap.begin(); it != callStmtProcMap.end(); ++it) {
		str += to_string((*it).first) + ": " + pt.getProcName((*it).second) + "\n";
	}
	SPALog::log(str);
}

void PKB::buildCFG(list<Statement> sourceCodeList) {
	cfg.buildGraph(sourceCodeList);
}

StatementType PKB::getType(int i)
{
	return cfg.getType(i);
}

int PKB::getStmtSize()
{
	return assignStmtList.size() + whileStmtList.size() + ifStmtList.size() + callStmtList.size();
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

void PKB::setVarTableReverse() {
	getVarTable().setVarTableReverse();
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

void PKB::setProcTableReverse() {
	getProcTable().setProcTableReverse();
}


//Modifies
void PKB::setModifies(int first, list<int> second) {
	getModifies().setModifies(first, second);
} // input parameter to be decided later

void PKB::resetModifies(int first, list<int> second) {
	getModifies().resetModifies(first, second);
}

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
	getModifies().logModifies(getProcTable(), getVarTable());
}

void PKB::setModifiesReverse() {
	getModifies().setModifiesReverse();
}


//Uses
void PKB::setUses(int first, list<int> second) {
	getUses().setUses(first, second);
} //input parameter to be decided later

void PKB::resetUses(int first, list<int> second) {
	getUses().resetUses(first, second);
}

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
	getUses().logUses(getProcTable(), getVarTable());
}

void PKB::setUsesReverse() {
	getUses().setUsesReverse();
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

void PKB::sortAndUnifyMap() {
	getPattern().sortAndUnifyMap();
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

void PKB::setFollowsReverse() {
	getFollows().setFollowsReverse();
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

void PKB::setFollowsStarReverse() {
	getFollowsStar().setFollowsStarReverse();
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
	getCalls().logCalls(getProcTable());
}

void PKB::sortAndUnifyCallsMap() {
	getCalls().sortAndUnifyMap();
}

map<int, list<int>> PKB::getCallsMap() {
	return getCalls().getCallsMap();
}

void PKB::setCallsReverse() {
	getCalls().setCallsReverse();
}


//CallsStar
void PKB::setCallsStar(int first, int second) {
	getCallsStar().setCallsStar(first, second);
}

list<int> PKB::getCallsStarFirst(int second) {
	return getCallsStar().getCallsStarFirst(second);
}

list<int> PKB::getCallsStarSecond(int first) {
	return getCallsStar().getCallsStarSecond(first);
}

bool PKB::isCallsStarValid(int first, int second) {
	return getCallsStar().isCallsStarValid(first, second);
}

void PKB::logCallsStar() {
	getCallsStar().logCallsStar(getProcTable());
}

void PKB::sortAndUnifyCallsStarMap() {
	getCallsStar().sortAndUnifyMap();
}

map<int, list<int>> PKB::getCallsStarMap() {
	return getCallsStar().getCallsStarMap();
}

void PKB::setCallsStarReverse() {
	getCallsStar().setCallsStarReverse();
}


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

void PKB::setParentReverse() {
	getParent().setParentReverse();
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

void PKB::setParentStarReverse() {
	getParentStar().setParentStarReverse();
}


//Next
list<int> PKB::getNextFirst(int second) {
	return cfg.getNextFirst(second);
}

list<int> PKB::getNextSecond(int first) {
	return cfg.getNextSecond(first);
}

bool PKB::isNextvalid(int first, int second) {
	return cfg.isNextValid(first, second);
}

void PKB::logNext() {
	return cfg.logNext();
}


//NextStar
list<int> PKB::getNextStarFirst(int second) {
	return cfg.getNextStarFirst(second);
}

list<int> PKB::getNextStarSecond(int first) {
	return cfg.getNextStarSecond(first);
}

bool PKB::isNextStarValid(int first, int second) {
	return cfg.isNextStarValid(first, second);
}

//Affect
list<int> PKB::getAffectsFirst(int end) {
	list<int> buffer;
	if (!isAssignment(end))
		return buffer;
	list<int> varUsesIndex = getUsesSecond(end);
	queue<int> path;
	vector<int> visit;
	string message;
	for (auto& tempUses : varUsesIndex)
	{
		clearQueue(path);
		visit.resize(getStmtSize() + 1, 0);
		visit[end] = -1;
		path.push(end);
		while (!path.empty())
		{
			int temp = path.front();
			path.pop();
			if (visit[temp] == -1)
			{
				//initial statement
				visit[temp] = 1;
				transfer(temp, path, false);
			}
			else
			{
				//check whether Modifies
				if (getType(temp) == assignmentStmt)
				{
					list<int> varModifiesIndex = getModifiesSecond(temp);
					if (contains(varModifiesIndex, tempUses))
					{
						buffer.push_back(temp);
					}
				}
				if (visit[temp] == 1)
				{
					//have visited 
					true;
				}
				else
				{
					transfer(temp, path, false);
					visit[temp] = 1;
				}
			}
		}
	}
	message = "Processing AffectFirst: the result for " +
		to_string(end) + " is:";
	_log.log(message);

	stringstream ss;
	for (auto& x : buffer)
	{
		ss << x << " ";
	}
	_log.log("  " + ss.str());
	return buffer;
}

list<int> PKB::getAffectsSecond(int start) {
	list<int> buffer;
	if (!isAssignment(start))
		return buffer;
	int varModifiesIndex = getModifiesSecond(start).front();
	queue<int> path;
	vector<int> visit;
	string message;
	visit.resize(getStmtSize() + 1, 0);
	visit[start] = -1;
	path.push(start);
	while (!path.empty())
	{
		int temp = path.front();
		path.pop();
		if (visit[temp] == -1)
		{
			//initial statement
			visit[temp] = 1;
			transfer(temp, path, true);
		}
		else
		{
			//check whether visit
			if (visit[temp] == 1)
			{
				//check whether uses
				if (getType(temp) == assignmentStmt)
				{
					list<int> varUsesIndex = getUsesSecond(temp);
					if (contains(varUsesIndex, varModifiesIndex))
					{
						buffer.push_back(temp);
					}
				}
			}
			else
			{
				visit[temp] = 1;
				//check whether is container
				if (isIf(temp) || isWhile(temp))
				{
					transfer(temp, path, true);
				}
				else
				{
					//check whether uses
					if (getType(temp) == assignmentStmt)
					{
						list<int> varUsesIndex = getUsesSecond(temp);
						if (contains(varUsesIndex, varModifiesIndex))
						{
							buffer.push_back(temp);
						}
					}
					//check whether modifies
					list<int> tempModifeis = getModifiesSecond(temp);
					if (!contains(tempModifeis, varModifiesIndex))
					{
						transfer(temp, path, true);
					}
					else
					{
						//this path cannot use anymore
					}
				}
			}
		}
	}
	message = "Processing AffectSecond: the result for " +
		to_string(start) + " is:";
	_log.log(message);

	stringstream ss;
	for (auto& x : buffer)
	{
		ss << x << " ";
	}
	_log.log("  " + ss.str());	return buffer;
}

bool PKB::isAffectsValid(int first, int second) {
	string message;
	if (!isAssignment(first) ||
		!isAssignment(second))
	{
		message = "Processing AffectValid:";
		_log.log(message);
		message = "(" + to_string(first)+ ", " + to_string(second) + 
			") statement type fault";
		_log.log(message);
		return false;
	}
	if (!isSameProc(first, second))
	{
		message = "Processing AffectValid:";
		_log.log(message);
		message = "(" + to_string(first) + ", " + to_string(second) +
			") different procedures";
		_log.log(message);
		return false;
	}
	int varModifiesIndex = getModifiesSecond(first).front();
	list<int> varUsesIndex = getUsesSecond(second);
	if (!contains(varUsesIndex, varModifiesIndex))
	{
		message = "Processing AffectValid:";
		_log.log(message);
		message = "(" + to_string(first) + ", " + to_string(second) +
			") variable not used";
		_log.log(message);
		return false;
	}
	queue<int> path;
	vector<int> visit;
	visit.resize(getStmtSize() + 1, 0);
	visit[first] = -1;
	path.push(first);
	while (!path.empty())
	{
		int temp = path.front();
		string message = "Processing Affect: reach " + to_string(temp);
		//_log.log(message);
		if (temp == second && visit[temp] != -1)
		{
			return true;
		}
		else if (visit[temp] == 1)
		{
			//have visited already
			//skip
		}
		else if (isWhile(temp))
		{
			path.pop();
			visit[temp] = 1;
			//the next line is not effective, try to optimize
			list<int> tempLst = getParentStarSecond(temp);
			assert(temp + 1 == tempLst.front());
			if (second <= tempLst.back() && second >= tempLst.front())
			{	//go into the loop
				path.push(temp + 1);
			}
			else
			{
				//get out of the loop
				list<int> tempNext = getNextSecond(temp);
				for (auto& x : tempNext)
				{
					if (x != temp + 1)
					{
						path.push(x);
					}
				}
			}
		}
		else if (isIf(temp))
		{
			//go to both statLst
			path.pop();
			visit[temp] = 1;
			list<int> tempNext = getNextSecond(temp);
			for (auto& x : tempNext)
			{
				path.push(x);
			}
		}
		else//assignStmt and callStmt
		{
			message = "Processing Affect: is Assignment/Call " + to_string(temp);
			//_log.log(message);
			path.pop();
			list<int> tempModifies = getModifiesSecond(temp);
			if (visit[temp] == -1 || !contains(tempModifies, varModifiesIndex))
			{
				list<int> tempNext = getNextSecond(temp);
				message = "Processing Affect: " + to_string(temp) +
					" has next " + to_string(tempNext.front());
				if (!tempNext.empty())
				{
					//_log.log(message);
					assert(tempNext.size() == 1);
					path.push(tempNext.front());
				}
			}
			visit[temp] = 1;
		}
	}
	return false;
}

//AffectStar
list<int> PKB::getAffectsStarFirst(int second) {
	return list<int>();
}

list<int> PKB::getAffectsStarSecond(int first) {
	return list<int>();
}

bool PKB::isAffectsStarValid(int first, int second) {
	return true;
}

//Some useful functions
bool PKB::isAssignment(int i)
{
	return getType(i) == assignmentStmt;
}

bool PKB::isWhile(int i)
{
	return getType(i) == whileStmt;
}

bool PKB::isIf(int i)
{
	return getType(i) == ifStmt;
}

bool PKB::isSameProc(int i, int j)
{
	int pro1 = getProcIDSpecial(i);
	int pro2 = getProcIDSpecial(j);
	return pro1 == pro2;
}

bool PKB::contains(list<int> lst, int i)
{
	for (auto& x : lst)
	{
		if (x == i)
			return true;
	}
	return false;
}
//mode equals true: push its next to queue,
//			  false: push its previous to queue
void PKB::transfer(int temp, queue<int>& q, bool mode)
{
	list<int> tempNext;
	if (mode)
	{
		tempNext = getNextSecond(temp);
	}
	else
	{
		tempNext = getNextFirst(temp);
	}
	if (!tempNext.empty())
	{
		for (auto& x : tempNext)
		{
			q.push(x);
		}
	}
}

void PKB::clearQueue(queue<int>& q)
{
	while (!q.empty())
	{
		q.pop();
	}
}