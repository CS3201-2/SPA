#include "SPALog.h"
#include <string>
#include <map>

using namespace std;


#ifndef Follows_H
#define Follows_H

class Follows{

public:
	Follows();

	void setFollows(int, int);
	int getFollowsFirst(int);
	int getFollowsSecond(int);
	bool isFollowValid(int, int);
	void logFollows();
	map<int, int> getFollowsMap();
	void setFollowsReverse();

private:
	map<int, int> followsMap;
	map<int, int> followsMapReverse;
};

#endif