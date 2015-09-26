#include <map>
#include <list>
#include <algorithm>
#include <iostream>
#include "Follows.h"

using namespace std;

class FollowsStar 
{
public: 
	FollowsStar();
	list<int> getFollowsStarStmt(int);
	list<int> getIsFollowedStarStmt(int);
	void createFollowsStarMap();
	void printFollowsStarMap();

private:
	Follows follows = Follows();
	map<int, list<int>> followsStarMap;
};