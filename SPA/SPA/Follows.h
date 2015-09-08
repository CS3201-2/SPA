#include <string>
#include <map>
#include <list>

using namespace std;

#ifndef Follows_H
#define Follows_H

class Follows
{
public:
	Follows();

	list<int> getFollowsStmt(int);

	void setFollowsStmt(int, list<int>);

private:
	// key is stmt line, value is a list of young brothers.
	map<int, std::list<int>> followsMap;
};

#endif
