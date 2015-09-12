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
	ResultTable(int isWholeTrue, vector<string> header, vector<vector<int>> result);
	void addTuple(vector<int>);   //store a line in to table
	vector<string> getHeader();
	vector<vector<int>> getResult();
	int getIsWholeTrue();
	int isWholeTrue;// -1 doesnt matter, 0 false, 1 true
	//when header is empty, check isWholeTrue. isWholeTrue is 1, skip to merge next table;
	//isWholeTrue is 0, return empty table, isWholeTrue set to 0
	//
	//when print result, isWholeTrue = 0, table is empty, print none
	//
	//select 23, 0, return none; 1, return 23; -1, check table and decidei

	vector<string> header;
	vector<vector<int>> result;
private:

};

#endif
