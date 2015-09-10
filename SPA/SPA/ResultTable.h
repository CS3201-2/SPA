#include <string>
#include <list>
#include <vector>

using namespace std;

#ifndef ResultTable_H
#define ResultTable_H

class ResultTable
{
public:
	ResultTable(string attr1, string attr2);
	void addTuple(vector<string>);   //store a line in to table

private:
	vector<string> header;
	vector<vector<string>> result;
};

#endif
