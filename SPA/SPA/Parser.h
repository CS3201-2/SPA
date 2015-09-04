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

	PKB parseSource(string);

	void trim(string& line);

	void addNewLineString(string& content);

	void buildSourceCodeList(string content, list<string>& list);


private:
	PKB pkb;

};

#endif
