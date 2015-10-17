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
	vector<bool> visit;
	visit.resize(_pkb->getStmtSize(), false);
	path.push(first);
	while (!path.empty())
	{
		//assume first != second first
		int temp = path.front();
		if (temp == second)
		{
			return true;
		}
		else if (visit[temp])
		{
			//have visited already
			//skip
		}
		else if (isWhile(temp))
		{
			path.pop();
			visit[temp] = true;
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
			if (!contains(tempModifies, varModifiesIndex))
			{
				list<int> tempNext = _pkb->getNextSecond(temp);
				if (!tempNext.empty())
				{
					assert(tempNext.size() == 1);
					path.push(tempNext.front());
				}
			}
		}
	}
	return false;
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
