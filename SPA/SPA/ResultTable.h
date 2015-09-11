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
	ResultTable(string attr1);
	ResultTable();
	void addTuple(vector<int>);   //store a line in to table
	int isWholeTrue;// -1 doesnt matter, 0 false, 1 true
	vector<string> getHeader();
	vector<vector<int>> getResult();
private:
	vector<string> header;
	vector<vector<int>> result;
};

#endif
