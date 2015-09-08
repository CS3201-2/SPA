#include "Parent.h"
#include <map>
#include <list>
#include <algorithm>

using namespace std;

Parent::Parent()
{
}

int Parent::getParentStmt(int stmtLine)
{
	return parentMap.at(stmtLine);
}

void Parent::setParentStmt(int parent, int child)
{
	parentMap[child] = parent;
}

