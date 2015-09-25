#include "PKB.h"
#include <string>
#include <list>

using namespace std;

#ifndef Controller_H
#define Controller_H

class Controller
{
public:
	Controller();

	void intializeCode( string );

	//void display( void );

	void processSource();

	list<string> processQuery(string query);
 
	void testingPKB();

private:
	string source;

	PKB ctrPKB;
	
	int syntaxCheck();

	PKB parse();
};

#endif