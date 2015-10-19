#include "Affect.h"



Affect::Affect()
{
	PKB* _pkb = PKB::getPKBInstance();
}

bool Affect::isAffectValid(int first, int second)
{
	if (!isAssignment(first) ||
		!isAssignment(second))
		return false;
	if (!isSameProc(first, second))
		return false;
	int varModifiesIndex = _pkb->getModifiesSecond(first).front();
	list<int> varUsesIndex = _pkb->getUsesSecond(second);
	if (!contains(varUsesIndex, varModifiesIndex))
		return false;
	queue<int> path;
	vector<int> visit;
	visit.resize(_pkb->getStmtSize(), 0);
	visit[first] == -1;
	path.push(first);
	while (!path.empty())
	{
		int temp = path.front();
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
			path.pop();
			list<int> tempModifies = _pkb->getModifiesSecond(temp);
			if (visit[temp] == -1 || !contains(tempModifies, varModifiesIndex))
			{
				list<int> tempNext = _pkb->getNextSecond(temp);
				if (!tempNext.empty())
				{
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
	visit.resize(_pkb->getStmtSize(), 0);
	visit[start] == -1;
	path.push(start);
	while (!path.empty())
	{
		int temp = path.front();
		if (visit[temp] == -1)
		{
			//initial statement
			visit[temp] == 1;
			list<int> tempNext = _pkb->getNextSecond(temp);
			if (!tempNext.empty())
			{
				assert(tempNext.size() == 1);
				path.push(tempNext.front());
			}
		}
		else
		{
			//check whether uses
			list<int> varUsesIndex = _pkb->getUsesSecond(temp);
			if (contains(varUsesIndex, varModifiesIndex))
			{
				buffer.push_back(temp);
			}
			//check whether visit
			if (visit[temp] == 1)
			{
				continue;
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
						//this path cannot use anymore
					}
				}
			}
		}
	}
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
