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
 
	void logPKB();

private:
	string source;

	PKB ctrPKB;
	
	bool syntaxCheck(list<pair<int, string>>&);

	PKB parse(list<pair<int, string>>);
};

#endif