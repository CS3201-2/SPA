#include "PKB.h"
#include <string>
#include <list>

using namespace std;

#ifndef Parser_H
#define Parser_H

class Parser
{
public:
	Parser();

	PKB parseSource(list<string>);

	string trim(string line);

	list<string> removeBlankLines(list<string>);

private:
	PKB pkb;

};

#endif
