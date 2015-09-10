#include <string>
#include <list>

using namespace std;

#ifndef ResultTable_H
#define ResultTable_H

class ResultTable
{
public:
	ResultTable(string attr1, string attr2);
	ResultTable mergeTable(ResultTable table_merged);

private:
	// attr1 and attr2 is table header
	bool _isTrueTable;
	string _attr1;
	string _attr2;
	list<pair<string,string>> tempResult;
};

#endif
