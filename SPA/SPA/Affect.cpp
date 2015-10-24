#include "Affect.h"

Affect::Affect()
{
	_pkb = PKB::getPKBInstance();
}

bool Affect::isAffectValid(int first, int second)
{
	if (!isAssignment(first) ||
		!isAssignment(second))
	{
		_log.log("Processing Affect: statement type fault");
		return false;
	}
	if (!isSameProc(first, second))
	{
		_log.log("Processing Affect: different procedures");
		return false;
	}	
	int varModifiesIndex = _pkb->getModifiesSecond(first).front();
	list<int> varUsesIndex = _pkb->getUsesSecond(second);
	if (!contains(varUsesIndex, varModifiesIndex))
	{
		_log.log("Processing Affect: variable not used");
		return false;
	}
	_log.log("Processing Affect: preconditions all met");
	queue<int> path;
	vector<int> visit;
	visit.resize(_pkb->getStmtSize()+1, 0);
	visit[first] = -1;
	path.push(first);
	while (!path.empty())
	{
		int temp = path.front();
		string message = "Processing Affect: reach " + to_string(temp);
		_log.log(message);
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
			list<int> tempLst = _pkb->getParentStarSecond(temp);
			assert(temp + 1 == tempLst.front());
			if (second <= tempLst.back() && second >= tempLst.front())
			{	//go into the loop
				path.push(temp + 1);
			}
			else
			{
				//get out of the loop
				list<int> tempNext = _pkb->getNextSecond(temp);
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
			list<int> tempNext = _pkb->getNextSecond(temp);
			for (auto& x : tempNext)
			{
				path.push(x);
			}
		}
		else//assignStmt and callStmt
		{
			message = "Processing Affect: is Assignment/Call " + to_string(temp);
			_log.log(message);
			path.pop();
			list<int> tempModifies = _pkb->getModifiesSecond(temp);
			if (visit[temp] == -1 || !contains(tempModifies, varModifiesIndex))
			{
				list<int> tempNext = _pkb->getNextSecond(temp);
				message = "Processing Affect: " + to_string(temp) +
					" has next " + to_string(tempNext.front());
				if (!tempNext.empty())
				{
					_log.log(message);
					assert(tempNext.size() == 1);
					path.push(tempNext.front());
				}
			}
			visit[temp] = 1;
		}
	}
	return false;
}

list<int> Affect::getAffectSecond(int start)
{
	list<int> buffer;
	if (!isAssignment(start))
		return buffer;
	int varModifiesIndex = _pkb->getModifiesSecond(start).front();
	queue<int> path;
	vector<int> visit;
	string message;
	visit.resize(_pkb->getStmtSize()+1, 0);
	visit[start] = -1;
	path.push(start);
	while (!path.empty())
	{
		int temp = path.front();
		_log.log("Processing Affect: now we are visiting " + to_string(temp));
		path.pop();
		if (visit[temp] == -1)
		{
			//initial statement
			visit[temp] = 1;
			list<int> tempNext = _pkb->getNextSecond(temp);
			if (!tempNext.empty())
			{
				assert(tempNext.size() == 1);
				path.push(tempNext.front());
			}
		}
		else
		{
			//check whether visit
			if (visit[temp] == 1)
			{
				//check whether uses
				if (_pkb->getType(temp) == assignmentStmt)
				{
					list<int> varUsesIndex = _pkb->getUsesSecond(temp);
					if (contains(varUsesIndex, varModifiesIndex))
					{
						buffer.push_back(temp);
					}
				}
			}
			else
			{
				visit[temp] == 1;
				//check whether is container
				if (isIf(temp) || isWhile(temp))
				{
					list<int> tempNext = _pkb->getNextSecond(temp);
					for (auto& x : tempNext)
					{
						path.push(x);
					}
				}
				else
				{
					//check whether uses
					if (_pkb->getType(temp) == assignmentStmt)
					{
						list<int> varUsesIndex = _pkb->getUsesSecond(temp);
						if (contains(varUsesIndex, varModifiesIndex))
						{
							buffer.push_back(temp);
						}
					}
					//check whether modifies
					list<int> tempModifeis = _pkb->getModifiesSecond(temp);
					if (!contains(tempModifeis, varModifiesIndex))
					{
						list<int> tempNext = _pkb->getNextSecond(temp);
						if (!tempNext.empty())
						{
							assert(tempNext.size() == 1);
							path.push(tempNext.front());
						}
					}
					else
					{
						message = "Processing Affect: " + to_string(start) + " is modified at " + to_string(temp);
						_log.log(message);
						//this path cannot use anymore
					}
				}
			}
		}
	}
	stringstream ss;
	for (auto& x : buffer)
	{
		ss << x << " ";
	}
	_log.log("Processing Affect: " + to_string(start) + " getAffectSecond is " + ss.str());
	return buffer;
}

Affect::~Affect()
{
}

bool Affect::isAssignment(int i)
{
	return _pkb->getType(i) == assignmentStmt;
}

bool Affect::isWhile(int i)
{
	return _pkb->getType(i) == whileStmt;
}

bool Affect::isIf(int i)
{
	return _pkb->getType(i) == ifStmt;
}

bool Affect::isSameProc(int i, int j)
{
	//havent implement
	return true;
}

bool Affect::contains(list<int> lst, int i)
{
	for (auto& x : lst)
	{
		if (x == i)
			return true;
	}
	return false;
}
