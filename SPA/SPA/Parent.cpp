#include "Parent.h"
#include <map>
#include <list>
#include <algorithm>

using namespace std;

Parent::Parent()
{
}

int Parent::getParent(int stmtLine)
{
	return parentMap.at(stmtLine);
}

void Parent::setParent(int parent, int child)
{
	parentMap[child] = parent;
}

