#include "SPALog.h"
#include <map>
#include <list>
#include <algorithm>

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
	void logFollowsStar();
	void sortAndUnifyMap();
	void setFollowsStarReverse();

private:
	map<int, list<int>> followsStarMap;
	map<int, list<int>> followsStarMapReverse;
};

#endif