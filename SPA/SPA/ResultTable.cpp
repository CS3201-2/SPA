#include "ResultTable.h"
#include <vector>
#include <string>

using namespace std;

ResultTable::ResultTable(string attr1, string attr2)
{
	header.push_back(attr1);
	header.push_back(attr2);
}

void ResultTable::addTuple(vector<string> tuple) {
	result.push_back(tuple);
}
