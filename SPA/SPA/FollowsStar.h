#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;


#ifndef FollowsStar_H
#define FollowsStar_H

class FollowsStar 
{
public: 
	FollowsStar();

	void setFollowsStar(int, list<int>);
	list<int> getFollowsStarFirst(int);
	list<int> getFollowsStarSecond(int);
	bool isFollowsStarValid(int, int);
	void printAllFollowsStar();

private:
	map<int, list<int>> followsStarMap;
};

#endif