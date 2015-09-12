#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include "ResultTable.h"
#include "PKB.h"

using namespace std;

#ifndef QueryResultProjector_H
#define QueryResultProjector_H

class QueryResultProjector
{
public:
	QueryResultProjector(list<ResultTable> resultList, string select, string selectType, PKB myPkb);
	list<string> getResult();

private:
	list<ResultTable> _resultList;
	ResultTable _resultTable;
	void mergeTable();
	int _isWholeTrue;// -1 doesnt matter, 0 false, 1 true
	string _select;
	string _selectType;
	PKB _myPkb;

	int getIndexOf(vector<string> header, string str);
	void createResultHeader(vector<string>& resultHeader, vector<string> header);
	void createResultTable(vector<vector<int>>& result, vector<vector<int>> table);
	list<std::pair<int, int>> getCommonPair(vector<string> header);
	vector<int> getIndexToRemove(list<std::pair<int, int>> commonPairList);
	void trimResultTable(vector<vector<int>>& result, list<std::pair<int, int>> commonPairList, vector<int> indexToRemove);
	void trimResultHeader(vector<string>& resultHeader, vector<int> indexToRemove);
};

#endif