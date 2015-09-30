#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef Follows_H
#define Follows_H

class Follows
{
public:
	Follows();

	void setFollows(int, int);

	//Follows (B, 8): B will be returned
	int getFollowsFirst(int);

	//Follows (8, A), A will be returned
	int getFollowsSecond(int);

	bool isFollowValid(int, int);

	void printFollowsMap();

	int getFollowsMapSize();

private:
	map<int, int> followsMap;
};

#endif