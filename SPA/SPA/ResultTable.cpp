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

ResultTable::ResultTable(int isWholeTrueA, vector<string> headerA, vector<vector<int>> resultA) 
{
	isWholeTrue = isWholeTrueA;
	header = headerA;
	result = resultA;
}

void ResultTable::addTuple(vector<int> tuple) {
	result.push_back(tuple);
}

vector<string> ResultTable::getHeader() {
	return header;
}

vector<vector<int>> ResultTable::getResult() {
	return result;
}

int ResultTable::getIsWholeTrue() {
	return isWholeTrue;
}
