#include <string>
#include <list>
#include <vector>
#include "ResultTable.h"
#include "PKB.h"

using namespace std;

#ifndef QueryResultProjector_H
#define QueryResultProjector_H

class QueryResultProjector {

public:
	QueryResultProjector(vector<ResultTable>, vector<ResultTable>, vector<string>, vector<string>);
	list<string> getResult();

private:
	int _isWholeTrue;// -1 doesnt matter, 0 false, 1 true
	vector<string> _select;
	vector<string> _selectType;
	map<string, int> _headerCount;
	vector<ResultTable> _selectTables;
	vector<ResultTable> _tempTables;
	ResultTable _finalTable;
	vector<int> _parent;

	ResultTable mergeTables();
	ResultTable mergeTwoTables(ResultTable, ResultTable);
	void countHeader();
	vector<int> getSelectIDsInFinalTable(vector<string>);
	vector<string> getCommonHeader(vector<string>, vector<string>);
	list<string> extractResultFromMergedTable();

	void logFinalResult(list<string>);

};

#endif