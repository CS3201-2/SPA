#include "PKB.h"
#include <string>
#include <list>

using namespace std;

#ifndef Controller_H
#define Controller_H

class Controller
{
public:
	Controller( list<string> );

	void display( void );

	void processSource();
 
private:
	list<string> source;

	PKB ctrPKB;
	
	int syntaxCheck();

	PKB parse();
};

#endif