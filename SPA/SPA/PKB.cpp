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
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL,str);
}

void PKB::logAssignList() {
	string str = "assign list\n";
	for (list<int>::iterator it = assignStmtList.begin(); it != assignStmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void PKB::logCallList() {
	string str = "call list\n";
	for (list<int>::iterator it = callStmtList.begin(); it != callStmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void PKB::logIfList() {
	string str = "if list\n";
	for (list<int>::iterator it = ifStmtList.begin(); it != ifStmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void PKB::logConstantList() {
	string str = "constant list\n";
	for (list<int>::iterator it = constantList.begin(); it != constantList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void PKB::logParentList() {
	string str = "parent list\n";
	list<int> parentList = getParentList();
	for (list<int>::iterator it = parentList.begin(); it != parentList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void PKB::logStmtList() {
	string str = "stmt list\n";
	list<int> stmtList = getStmtList();
	for (list<int>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		str += to_string(*it) + ", ";
	}
	str += "\n";
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
}

void PKB::logCallStmtProcMap() {
	string str = "call stmt map\n";
	ProcTable pt = getProcTable();
	for (map<int, int>::iterator it = callStmtProcMap.begin(); it != callStmtProcMap.end(); ++it) {
		str += to_string((*it).first) + ": " + pt.getProcName((*it).second) + "\n";
	}
	SPALog::getSPALogInstance()->logWithLevel(MEDIUM_LEVEL, str);
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
int PKB::insertVar(VARNAME varName) {
	return getVarTable().insertVar(varName);
}

int PKB::getVarID(VARNAME varName) {
	return getVarTable().getVarID(varName);
}

string PKB::getVarName(VARIABID varID) {
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
int PKB::insertProc(PROCNAME procName) {
	return getProcTable().insertProc(procName);
}

int PKB::getProcID(PROCNAME procName) {
	return getProcTable().getProcID(procName);
} 

string PKB::getProcName(PROCID procID) {
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
void PKB::setModifies(INDEX first, list<VARIABID> second) {
	getModifies().setModifies(first, second);
}

void PKB::resetModifies(INDEX first, list<VARIABID> second) {
	getModifies().resetModifies(first, second);
}

list<INDEX> PKB::getModifiesFirst(VARIABID second) {
	return getModifies().getModifiesFirst(second);
}

list<VARIABID> PKB::getModifiesSecond(INDEX first) {
	return getModifies().getModifiesSecond(first);
}

bool PKB::isModifiesValid(INDEX first, VARIABID second) {
	return getModifies().isModifiesValid(first, second);
}

void PKB::logModifies() {
	getModifies().logModifies(getProcTable(), getVarTable());
}

void PKB::setModifiesReverse() {
	getModifies().setModifiesReverse();
}


//Uses
void PKB::setUses(INDEX first, list<int> second) {
	getUses().setUses(first, second);
}

void PKB::resetUses(INDEX first, list<int> second) {
	getUses().resetUses(first, second);
}

list<INDEX> PKB::getUsesFirst(VARIABID second) {
	return getUses().getUsesFirst(second);
}

list<int> PKB::getUsesSecond(INDEX first) {
	return getUses().getUsesSecond(first);
}

bool PKB::isUsesValid(INDEX first, VARIABID second) {
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
void PKB::setFollows(STMTLINE first, STMTLINE second) {
	getFollows().setFollows(first, second);
}

STMTLINE PKB::getFollowsFirst(STMTLINE second) {
	return getFollows().getFollowsFirst(second);
}

STMTLINE PKB::getFollowsSecond(STMTLINE first) {
	return getFollows().getFollowsSecond(first);
}

bool PKB::isFollowsValid(STMTLINE first, STMTLINE second) {
	return getFollows().isFollowValid(first, second);
}

void PKB::logFollows() {
	getFollows().logFollows();
}

map<STMTLINE, STMTLINE> PKB::getFollowsMap() {
	return getFollows().getFollowsMap();
}

void PKB::setFollowsReverse() {
	getFollows().setFollowsReverse();
}


//FollowsStar
void PKB::setFollowsStar(STMTLINE first, list<STMTLINE> second) {
	getFollowsStar().setFollowsStar(first, second);
}

list<STMTLINE> PKB::getFollowsStarFirst(STMTLINE second) {
	return getFollowsStar().getFollowsStarFirst(second);
}

list<STMTLINE> PKB::getFollowsStarSecond(STMTLINE first) {
	return getFollowsStar().getFollowsStarSecond(first);
}

bool PKB::isFollowsStarValid(STMTLINE first, STMTLINE second) {
	return getFollowsStar().isFollowsStarValid(first, second);
}

void PKB::logFollowsStar() {
	getFollowsStar().logFollowsStar();
}

void PKB::setFollowsStarReverse() {
	getFollowsStar().setFollowsStarReverse();
}


//Calls
void PKB::setCalls(PROCID first, PROCID second) {
	getCalls().setCalls(first, second);
}

list<PROCID> PKB::getCallsFirst(PROCID second) {
	return getCalls().getCallsFirst(second);
}

list<PROCID> PKB::getCallsSecond(PROCID first) {
	return getCalls().getCallsSecond(first);
}

bool PKB::isCallsValid(PROCID first, PROCID second) {
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
void PKB::setCallsStar(PROCID first, list<PROCID> second) {
	getCallsStar().setCallsStar(first, second);
}

list<PROCID> PKB::getCallsStarFirst(PROCID second) {
	return getCallsStar().getCallsStarFirst(second);
}

list<PROCID> PKB::getCallsStarSecond(PROCID first) {
	return getCallsStar().getCallsStarSecond(first);
}

bool PKB::isCallsStarValid(PROCID first, PROCID second) {
	return getCallsStar().isCallsStarValid(first, second);
}

void PKB::logCallsStar() {
	getCallsStar().logCallsStar(getProcTable());
}

void PKB::sortAndUnifyCallsStarMap() {
	getCallsStar().sortAndUnifyMap();
}

map<PROCID, list<PROCID>> PKB::getCallsStarMap() {
	return getCallsStar().getCallsStarMap();
}

void PKB::setCallsStarReverse() {
	getCallsStar().setCallsStarReverse();
}


//Parent
void PKB::setParent(STMTLINE first, list<STMTLINE> second) {
	getParent().setParent(first, second);
}

STMTLINE PKB::getParentFirst(STMTLINE second) {
	return getParent().getParentFirst(second);
}

list<STMTLINE> PKB::getParentSecond(STMTLINE first) {
	return getParent().getParentSecond(first);
}

bool PKB::isParentValid(STMTLINE first, STMTLINE second) {
	return getParent().isParentValid(first, second);
}

void PKB::logParent() {
	getParent().logParent();
}

map<STMTLINE, list<STMTLINE>> PKB::getParentMap() {
	return getParent().getParentMap();
}

void PKB::setParentReverse() {
	getParent().setParentReverse();
}


//ParentStar
void PKB::setParentStar(STMTLINE first, list<STMTLINE> second) {
	getParentStar().setParentStar(first, second);
}

list<STMTLINE> PKB::getParentStarFirst(STMTLINE second) {
	return getParentStar().getParentStarFirst(second);
}

list<STMTLINE> PKB::getParentStarSecond(STMTLINE first) {
	return getParentStar().getParentStarSecond(first);
}

bool PKB::isParentStarValid(STMTLINE first, STMTLINE second) {
	return getParentStar().isParentStarValid(first, second);
}

void PKB::logParentStar() {
	getParentStar().logParentStar();
}

void PKB::setParentStarReverse() {
	getParentStar().setParentStarReverse();
}


//Next
list<STMTLINE> PKB::getNextFirst(STMTLINE second) {
	return cfg.getNextFirst(second);
}

list<STMTLINE> PKB::getNextSecond(STMTLINE first) {
	return cfg.getNextSecond(first);
}

bool PKB::isNextvalid(STMTLINE first, STMTLINE second) {
	return cfg.isNextValid(first, second);
}

void PKB::logNext() {
	return cfg.logNext();
}


//NextStar
list<STMTLINE> PKB::getNextStarFirst(STMTLINE second) {
	return cfg.getNextStarFirst(second);
}

list<STMTLINE> PKB::getNextStarSecond(STMTLINE first) {
	return cfg.getNextStarSecond(first);
}

bool PKB::isNextStarValid(STMTLINE first, STMTLINE second) {
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
		message = "Processing AffectFirst: dealing with " + to_string(tempUses);
		_log.logWithLevel(4, message);
		clearQueue(path);
		visit.resize(getStmtSize() + 1, 0);
		visit.assign(visit.size(), 0);
		visit[end] = -1;
		path.push(end);
		while (!path.empty())
		{
			int temp = path.front();
			message = "Processing AffectFirst: reach " + to_string(temp);
			_log.logWithLevel(4, message);
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
					else
					{
						transfer(temp, path, false);
					}
					visit[temp] = 1;
				}
				else if (getType(temp) == ifStmt ||
					getType(temp)==whileStmt)
				{
					if (visit[temp] != 1)
					{
						transfer(temp, path, false);
						visit[temp] = 1;
					}
				}
				else
				{
					if (visit[temp] != 1)
					{
						list<int> varModifiesIndex = getModifiesSecond(temp);
						if (!contains(varModifiesIndex, tempUses))
						{
							transfer(temp, path, false);
						}
						visit[temp] = 1;
					}
				}
			}
		}
	}
	message = "Processing AffectFirst: the result for " +
		to_string(end) + " is:";
	_log.logWithLevel(4, message);

	stringstream ss;
	for (auto& x : buffer)
	{
		ss << x << " ";
	}
	_log.logWithLevel(4, "  " + ss.str());
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
		string message = "Processing AffectsSecond: reach " + to_string(temp);
		_log.logWithLevel(4, message);
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
	_log.logWithLevel(4, message);

	stringstream ss;
	for (auto& x : buffer)
	{
		ss << x << " ";
	}
	_log.logWithLevel(4, "  " + ss.str());	
	return buffer;
}

bool PKB::isAffectsValid(int first, int second) {
	string message;
	if (!isAssignment(first) ||
		!isAssignment(second))
	{
		message = "Processing AffectValid:";
		_log.logWithLevel(4, message);
		message = "(" + to_string(first)+ ", " + to_string(second) + 
			") statement type fault";
		_log.logWithLevel(4, message);
		return false;
	}
	if (!isSameProc(first, second))
	{
		message = "Processing AffectValid:";
		_log.logWithLevel(4, message);
		message = "(" + to_string(first) + ", " + to_string(second) +
			") different procedures";
		_log.logWithLevel(4, message);
		return false;
	}
	int varModifiesIndex = getModifiesSecond(first).front();
	list<int> varUsesIndex = getUsesSecond(second);
	if (!contains(varUsesIndex, varModifiesIndex))
	{
		message = "Processing AffectValid:";
		_log.logWithLevel(4, message);
		message = "(" + to_string(first) + ", " + to_string(second) +
			") variable not used";
		_log.logWithLevel(4, message);
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
		path.pop();
		string message = "Processing AffectsValid: reach " + to_string(temp);
		_log.logWithLevel(4, message);
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
			visit[temp] = 1;
			transfer(temp, path, true);
		}
		else//assignStmt and callStmt
		{
			list<int> tempModifies = getModifiesSecond(temp);
			if (visit[temp] == -1 || !contains(tempModifies, varModifiesIndex))
			{
				transfer(temp, path, true);
				
			}
			visit[temp] = 1;
		}
	}
	return false;
}

//AffectStar
list<int> PKB::getAffectsStarFirst(int second) {
	list<int> buffer;
	list<int> temp;
	list<int> todo;
	string message;
	temp = getAffectsFirst(second);
	temp.sort();
	todo = insertAll(buffer, temp);
	while (!todo.empty())
	{
		for (auto& x : todo)
		{
			message = "Processing AffectStarFirst: visiting "
				+ to_string(x);
			_log.logWithLevel(4, message);
			list<int> y = getAffectsFirst(x);
			y.sort();
			insertAll(temp, y);
		}
		todo = insertAll(buffer, temp);
	}
	return buffer;
}

list<int> PKB::getAffectsStarSecond(int first) {
	list<int> buffer;
	list<int> temp;
	list<int> todo;
	string message;
	temp = getAffectsSecond(first);
	temp.sort();
	todo = insertAll(buffer, temp);
	while (!todo.empty())
	{
		for (auto& x : todo)
		{
			message = "Processing AffectStarSecond: visiting "
				+ to_string(x);
			_log.logWithLevel(4, message);
			list<int> y = getAffectsSecond(x);
			y.sort();
			insertAll(temp, y);
		}
		todo = insertAll(buffer, temp);
	}
	return buffer;
}

bool PKB::isAffectsStarValid(int first, int second) {
	list<int> buffer;
	list<int> temp;
	list<int> todo;
	string message;
	temp = getAffectsSecond(first);
	temp.sort();
	todo = insertAll(buffer, temp);
	while (!todo.empty())
	{
		for (auto& x : todo)
		{
			message = "Processing AffectStarValid: visiting "
				+ to_string(x);
			_log.logWithLevel(4, message);
			if (x == second)
				return true;
			list<int> y = getAffectsSecond(x);
			y.sort();
			insertAll(temp, y);
		}
		todo = insertAll(buffer, temp);
	}
	return false;
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

list<int> PKB::merge(list<int>& lst1, list<int>& lst2)
{
	//merge two sorted list and return different elements;
	list<int>::iterator it1 = lst1.begin();
	list<int>::iterator it2 = lst2.begin();
	list<int> newElements;
	while (it1 != lst1.end() && it2 != lst2.end())
	{
		int element1 = *it1;
		int element2 = *it2;
		if (element1 < element2)
		{
			it1++;
		}
		else if (element1 > element2)
		{
			lst1.insert(it1, element2);
			newElements.push_back(element2);
			it2++;
		}
		else
		{
			it1++;
			it2++;
		}
	}
	if (it2 != lst2.end())
	{
		while (it2 != lst2.end())
		{
			lst1.push_back(*it2);
			newElements.push_back(*it2);
			it2++;
		}
	}
	lst2.clear();
	return newElements;
}

list<int>::iterator PKB::search(list<int>::iterator first, 
	list<int>::iterator last, int value)
{
	list<int>::difference_type len = distance(first, last);
	while (len > 0)
	{
		list<int>::iterator i = first;
		list<int>::difference_type len2 = len / 2;
		advance(i, len2);
		if (*i < value)
		{
			first = ++i;
			len -= len2 + 1;
		}
		else
		{
			len = len2;
		}
	}
	return first;
}

list<int> PKB::insertAll(list<int>& lst1, list<int>& lst2)
{
	list<int>::iterator begin = lst1.begin();
	list<int>::iterator end = lst1.end();
	list<int> newElements;
	for (auto& x : lst2)
	{
		list<int>::iterator tempIt = search(begin, end, x);
		if (tempIt == end || *tempIt != x)
		{
			lst1.insert(tempIt, x);
			newElements.push_back(x);
		}
		else
		{
			//x is already in the lst
		}
		begin = tempIt;
	}
	return newElements;
}

