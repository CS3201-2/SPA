#include "Follows.h"
#include <map>
#include <list>
#include <algorithm>

using namespace std;

// constructor
Follows::Follows()
{
}

list<int> Follows::getFollows(int stmt_line)
{
	return followsMap.at(stmt_line);
}

void Follows::setFollows(int stmt_Line, list<int> stmt_Lines)
{
	followsMap[stmt_Line] = stmt_Lines;

}

