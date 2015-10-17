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
	bool success = false;
	queue<int> path;
	vector<bool> visit;
	visit.resize(_pkb->getStatListSize(), false);

	return false;
}

Affect::~Affect()
{
}

bool Affect::isAssignment(int i)
{
	//return _pkb->getType(i) == StatementType::assignmentStmt;
	return true;
}

bool Affect::isWhile(int i)
{
	//have not implemented
	return true;
}

bool Affect::isIf(int i)
{
	//have not implemented
	return false;
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
