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
	QueryResultProjector(vector<ResultTable>, vector<string>, vector<string>);
	QueryResultProjector(vector<ResultTable>, vector<ResultTable>, vector<string>, vector<string>);
	list<string> getResultOld();
	list<string> getResult();

private:
	int _isWholeTrue;// -1 doesnt matter, 0 false, 1 true
	vector<ResultTable> _oneVarTables;
	vector<ResultTable> _twoVarTables;
	vector<string> _select;
	vector<string> _selectType;
	map<string, int> _headerCount;
	vector<ResultTable> _tempTables;
	ResultTable _resultTable;
	
	void logFinalResult(list<string>);
	ResultTable mergeTables();
	ResultTable mergeTables(vector<int>);
	ResultTable mergeTwoTables(ResultTable, ResultTable);
	vector<int> getMergingOrder();
	//might need to deal with "_" in the table header
	//void trimTempTables();
	void trimTempTablesOld();
	void countHeader();
	bool isSelectBool();
	vector<int> getIndexOfSelect(vector<string>);
	vector<string> getCommonHeader(vector<string>, vector<string>);
};

#endif