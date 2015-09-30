#include <string>
#include <iostream>
#include <map>

using namespace std;


#ifndef Follows_H
#define Follows_H

class Follows
{
public:
	Follows();

	void setFollows(int, int);
	int getFollowsFirst(int);
	int getFollowsSecond(int);
	bool isFollowValid(int, int);
	void printFollowsMap();

private:
	map<int, int> followsMap;
};

#endif