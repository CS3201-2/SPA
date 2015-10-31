#include"PKB.h"
#include<sstream>
#include<assert.h>
using namespace std;
#ifndef Affect_H
#define Affect_H
class Affect
{
	PKB* _pkb;
	SPALog _log;
public:
	Affect();
	bool isAffectValid(int,int);
	list<int> getAffectFirst(int);
	list<int> getAffectSecond(int);
	~Affect();
private:
	bool isAssignment(int);
	bool isWhile(int);
	bool isIf(int);
	bool isSameProc(int, int);
	bool contains(list<int>, int);
	void transfer(int,queue<int>&, bool);
	void clearQueue(queue<int>&);
};
#endif
