#include "ResultTable.h"
#include <vector>
#include <string>

using namespace std;

ResultTable::ResultTable() {
	isWholeTrue = -1;
}

ResultTable::ResultTable(string attr1, string attr2)
{
	isWholeTrue = -1;
	header.push_back(attr1);
	header.push_back(attr2);
}

ResultTable::ResultTable(string attr1)
{
	isWholeTrue = -1;
	header.push_back(attr1);
}

void ResultTable::addTuple(vector<int> tuple) {
	result.push_back(tuple);
}
