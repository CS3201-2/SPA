#include"PKB.h"

using namespace std;
#ifndef Affect_H
#define Affect_H
class Affect
{
	PKB* _pkb;
public:
	Affect();
	bool isAffectValid(int,int);
	list<int> getAffectFirst(int);
	list<int> getAffectSecond(int);
	~Affect();
};
#endif