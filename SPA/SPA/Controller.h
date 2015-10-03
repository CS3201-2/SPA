#include "PKB.h"
#include "DesignExtractor.h"
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
	
	void logSourceCode(list<pair<int, string>>);
	bool syntaxCheck(list<pair<int, string>>&);
};

#endif