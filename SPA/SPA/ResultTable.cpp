#include "ResultTable.h"


using namespace std;

ResultTable::ResultTable() {
	_isWholeTrue = -1;
}

ResultTable::ResultTable(string attr1, string attr2) {
	_isWholeTrue = -1;
	_header.push_back(attr1);
	_header.push_back(attr2);
}

ResultTable::ResultTable(string attr1) {
	_isWholeTrue = -1;
	_header.push_back(attr1);
}

ResultTable::ResultTable(int isWholeTrue, vector<string> header, vector<vector<int>> result) {
	_isWholeTrue = isWholeTrue;
	_header = header;
	_result = result;
}

void ResultTable::addTuple(vector<int> tuple) {
	_result.push_back(tuple);
}

vector<string> ResultTable::getHeader() {
	return _header;
}

vector<vector<int>> ResultTable::getContent() {
	return _result;
}

size_t ResultTable::getTableSize() {
	return _result.size();
}

int ResultTable::getIsWholeTrue() {
	return _isWholeTrue;
}

void ResultTable::setIsWholeTrue(int isWholeTrue) {
	_isWholeTrue = isWholeTrue;
}

void ResultTable::logTable(int index) {
	string str;

	str += "table No." + to_string(index);

	for (auto&x : _header) {
		str += x + " ";
	}
	str += "\n";

	for (auto&x : _result) {
		for (auto&y : x) {
			str += to_string(y) + " ";
		}
		str += "\n";
	}
	str += "--------------------------------\n";

	SPALog::log(str);
}