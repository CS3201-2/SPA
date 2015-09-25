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

	void setFollowsStmt(int, int);

	//Follows (8, A), A will be returned
	int getFollowsStmt(int);

	//Follows (B, 8): B will be returned
	int getIsFollowedStmt(int);

	void printFollowsMap();

	int getFollowsMapSize();

private:
	map<int, int> followsMap;
};

#endif