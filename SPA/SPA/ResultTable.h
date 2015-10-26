#include <string>
#include <list>
#include <vector>
#include "SPALog.h"

using namespace std;

#ifndef ResultTable_H
#define ResultTable_H

class ResultTable {
public:
	ResultTable(string attr1, string attr2);
	ResultTable(string attr1);
	ResultTable();
	ResultTable(int isWholeTrue, vector<string> header, vector<vector<int>> result);
	
	void addTuple(vector<int>);   //store a line in to table
	vector<string> getHeader();
	vector<vector<int>> getResult();
	int getIsWholeTrue();
	void setIsWholeTrue(int);
	int isWholeTrue;
	void logTable();

private:
	vector<string> _header;
	vector<vector<int>> _result;
	int _isWholeTrue;
};
#endif
