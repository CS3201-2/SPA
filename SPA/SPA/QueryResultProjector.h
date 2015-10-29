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
	//for select non boolean things
	//as for the list of tables, put all twoVarTables first
	//then append those oneVarTables at the back if their header 
	//is part of the select clause
	
	//just put all tables in the list, it would be better if all twoVarTables
	//are at the front of the list
	QueryResultProjector(vector<ResultTable>, vector<string>, vector<string>);
	list<string> getResult();

private:
	int _isWholeTrue;// -1 doesnt matter, 0 false, 1 true
	vector<string> _select;
	vector<string> _selectType;
	map<string, int> _headerCount;
	vector<ResultTable> _tempTables;
	ResultTable _finalTable;
	
	ResultTable mergeTables();
	//ResultTable mergeTables(vector<int>);
	ResultTable mergeTwoTables(ResultTable, ResultTable);
	//vector<int> getMergingOrder();
	//void trimTempTables();
	//void trimTempTablesOld();
	//void countHeader();
	bool isSelectBoolType();
	vector<int> getSelectIDsInFinalTable(vector<string>);
	vector<string> getCommonHeader(vector<string>, vector<string>);
	list<string> extractResultFromMergedTable();

	void logFinalResult(list<string>);

};

#endif