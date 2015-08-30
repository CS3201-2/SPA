#pragma once
#include <string>
#include <list>

using namespace std;

#ifndef Parser_H
#define Parser_H

class Parser
{
public:
	Parser();

	PKB parseSource( list<string> );

private:
	PKB pkb;

	list<string> trim( list<string> );

	list<string> removeBlankLines( list<string> );

};

#endif
