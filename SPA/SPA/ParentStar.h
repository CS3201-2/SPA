#include <string>
#include <list>
#include <map>
#include <iostream>

using namespace std;


class ParentStar 
{
public:
	ParentStar();

	void setParentStar(int, list<int>);
	list<int> getParentStarFirst(int);
	list<int> getParentStarSecond(int);
	bool isParentStarValid(int, int);
	void printAllParentStar();

private:
	map<int, list<int>> parentStarMap;
};