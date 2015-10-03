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
 

private:
	string source;

	void logPKB();
	void logSourceCode(list<pair<int, string>>);
	bool syntaxCheck(list<pair<int, string>>&);
};

#endif