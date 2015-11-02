#include "QueryResultProjector.h"

const string TYPE_VARIABLE = "variable";
const string TYPE_PROCEDURE = "procedure";
const string TYPE_BOOL = "boolean";
const string RESULT_TRUE = "true";
const string RESULT_FALSE = "false";
const int FIRST_TYPE = 0;
const size_t HEADER_SIZE_ONE = 1;
const size_t HEADER_SIZE_TWO = 2;
const size_t HEADER_SIZE_ZERO = 0;
const int FIRST_COMMON_HEADER = 0;
const int SECOND_COMMON_HEADER = 1;
const int EMPTY_TABLE = 0;

//constructor
QueryResultProjector::QueryResultProjector(vector<ResultTable> tempTables, 
	vector<string> select, vector<string> selectType) {
	_tempTables = tempTables;
	_isWholeTrue = -1;
	_select = select;
	_selectType = selectType;
}

list<string> QueryResultProjector::getResult() {
	list<string> resultStringList;

	if (_selectType[FIRST_TYPE] == TYPE_BOOL) {
		if (_tempTables.empty()) {
			resultStringList.push_back(RESULT_TRUE);
		}
		else {
			_finalTable = mergeTables();
			if (_finalTable.getTableSize() == EMPTY_TABLE) {
				resultStringList.push_back(RESULT_FALSE);
			}
			else {
				resultStringList.push_back(RESULT_TRUE);
			}
		}
	}
	else {
		_finalTable = mergeTables();
		_finalTable.logTable(-1);
		resultStringList = extractResultFromMergedTable();
	}


	return resultStringList;
}

map<int, vector<int>> QueryResultProjector::getMergingOrderBoolean() {
	map<int, vector<int>> mergingOrder;
	for (size_t i = 0; i < _parent.size(); ++i) {
		vector<int> temp;
		if (mergingOrder.find(_parent[i]) == mergingOrder.end()) {
			temp.push_back(i);
			mergingOrder[_parent[i]] = temp;
		}
		else {
			temp = mergingOrder.at(_parent[i]);
			temp.push_back(i);
			mergingOrder[_parent[i]] = temp;
		}
	}

	return mergingOrder;
}

//assume there is at least one table in _tempTables
ResultTable QueryResultProjector::mergeTables() {
	ResultTable finalTable, tempTable;
	finalTable = _tempTables.at(0);

	for (size_t i = 1; i < _tempTables.size(); ++i) {
		tempTable = _tempTables.at(i);
		finalTable = mergeTwoTables(finalTable, tempTable);

		if (finalTable.getTableSize() == EMPTY_TABLE) {
			return ResultTable();
		}
	}

	return finalTable;
}

void QueryResultProjector::countHeader() {
	for (auto &i : _tempTables) {
		vector<string> header = i.getHeader();
		for (auto &j : header) {
			if (_headerCount.find(j) == _headerCount.end()) {
				_headerCount[j] = 1;
			}
			else {
				int prevCount = _headerCount.at(j);
				_headerCount[j] = prevCount + 1;
			}
		}
	}
}

ResultTable QueryResultProjector::mergeTwoTables(ResultTable r1, ResultTable r2) {
	//r1 is always the table to be hashed into the unordered map
	//size of header of r2 can only be 1 or 2
	unordered_map<int, list<vector<int>>> hashedMap;
	vector<string> cHeaders, rHeader, header1 = r1.getHeader(), header2 = r2.getHeader();
	vector<vector<int>> rContent, content1 = r1.getContent(), content2 = r2.getContent();
	string cHeader1, cHeader2;
	int cHeader1InH1ID, cHeader1InH2ID, cHeader2InH1ID, cHeader2InH2ID;

	cHeaders = getCommonHeader(header1, header2);
	//there is at least one common header in both tables, not handled the case where there is two common headers
	if (!cHeaders.empty()) {
		string cHeader1 = cHeaders.at(FIRST_COMMON_HEADER);

		//select r1 as the table to be hashed into the map
		for (int i = 0; i < header1.size(); ++i) {
			if (header1.at(i) == cHeader1) {
				cHeader1InH1ID = i;
				break;
			}
		}

		header1.erase(header1.begin() + cHeader1InH1ID);

		for (vector<vector<int>>::iterator it = content1.begin(); it != content1.end(); ++it) {
			vector<int> tempResult = *it;
			int key = tempResult.at(cHeader1InH1ID);
			tempResult.erase(tempResult.begin() + cHeader1InH1ID);
			list<vector<int>> temps;
			if (hashedMap.find(key) == hashedMap.end()) {
				temps.push_back(tempResult);
				hashedMap[key] = temps;
			}
			else {
				temps = hashedMap.at(key);
				temps.push_back(tempResult);
				hashedMap[key] = temps;
			}
		}
		//end of hashing into hashedMaps

		if (cHeaders.size() == HEADER_SIZE_ONE) {
			//get the index to remove in the second table
			for (int i = 0; i < header2.size(); ++i) {
				if (header2.at(i) == cHeader1) {
					cHeader1InH2ID = i;
					break;
				}
			}

			header2.erase(header2.begin() + cHeader1InH2ID);
			//construct result header
			rHeader.push_back(cHeader1);
			rHeader.insert(rHeader.end(), header1.begin(), header1.end());
			rHeader.insert(rHeader.end(), header2.begin(), header2.end());
			for (vector<vector<int>>::iterator it = content2.begin(); it != content2.end(); ++it) {
				vector<int> tempResult = *it;
				int probingKey = tempResult.at(cHeader1InH2ID);
				tempResult.erase(tempResult.begin() + cHeader1InH2ID);
				if (hashedMap.find(probingKey) != hashedMap.end()) {
					list<vector<int>> probingResult = hashedMap.at(probingKey);
					for (list<vector<int>>::iterator it2 = probingResult.begin(); it2 != probingResult.end(); ++it2) {
						vector<int> resultTuple;
						resultTuple.push_back(probingKey);
						resultTuple.insert(resultTuple.end(), (*it2).begin(), (*it2).end());
						resultTuple.insert(resultTuple.end(), tempResult.begin(), tempResult.end());
						rContent.push_back(resultTuple);
					}
				}
			}
		}
		else if(cHeaders.size() == HEADER_SIZE_TWO){
			cHeader2 = cHeaders.at(SECOND_COMMON_HEADER);
			for (int i = 0; i < HEADER_SIZE_TWO; ++i) {
				if (header2[i] == cHeader1) {
					cHeader1InH2ID = i;
				}
				if (header2[i] == cHeader2) {
					cHeader2InH2ID = i;
				}
			}

			for (int i = 0; i < header1.size(); ++i) {
				if (header1[i] == cHeader2) {
					cHeader2InH1ID = i;
					break;
				}
			}

			rHeader.push_back(cHeader1);
			rHeader.insert(rHeader.end(), header1.begin(), header1.end());

			for (vector<vector<int>>::iterator it = content2.begin(); it != content2.end(); ++it) {
				int probingKey = (*it).at(cHeader1InH2ID);
				if (hashedMap.find(probingKey) != hashedMap.end()) {
					list<vector<int>> probingResult = hashedMap.at(probingKey);
					for (list<vector<int>>::iterator it2 = probingResult.begin(); it2 != probingResult.end(); ++it2) {
						if ((*it2)[cHeader2InH1ID] == (*it)[cHeader2InH2ID]) {
							vector<int> resultTuple;
							resultTuple.push_back(probingKey);
							resultTuple.insert(resultTuple.end(), (*it2).begin(), (*it2).end());
							rContent.push_back(resultTuple);
						}
					}
				}
			}
		}
	}
	//no common header, but still need to merge table, for Select Boolean
	else {
		rHeader.insert(rHeader.end(), header1.begin(), header1.end());
		rHeader.insert(rHeader.end(), header2.begin(), header2.end());
		for (vector<vector<int>>::iterator it1 = content1.begin(); it1 != content1.end(); ++it1) {
			for (vector<vector<int>>::iterator it2 = content2.begin(); it2 != content2.end(); ++it2) {
				vector<int> resultTuple;
				resultTuple.insert(resultTuple.end(), (*it1).begin(), (*it1).end());
				resultTuple.insert(resultTuple.end(), (*it2).begin(), (*it2).end());
				rContent.push_back(resultTuple);
			}
		}
	}

	return ResultTable(-1, rHeader, rContent);
}

vector<string> QueryResultProjector::getCommonHeader(vector<string> h1, vector<string> h2) {
	vector<string> cHeader;

	for (auto &i : h1) {
		for (auto &j : h2) {
			if (i == j) {
				cHeader.push_back(i);
			}
		}
	}

	return cHeader;
}

vector<int> QueryResultProjector::getSelectIDsInFinalTable(vector<string> finalTableHeader) {
	vector<int> index;

	for (auto &i : _select) {
		for (int j = 0; j < finalTableHeader.size(); ++j) {
			if (i == finalTableHeader[j]) {
				index.push_back(j);
				break;
			}
		}
	}

	return index;
}

list<string> QueryResultProjector::extractResultFromMergedTable() {
	list<string> resultList;

	if (_finalTable.getTableSize() != 0) {
		vector<int> selectIDsInFinalTable = getSelectIDsInFinalTable(_finalTable.getHeader());
		vector<vector<int>> finalContent = _finalTable.getContent();
		//tuple case is not fully handled, eg Select<a, a, a>
		for (int i = 0; i < finalContent.size(); ++i) {
			string result = "";
			for (int j = 0; j < selectIDsInFinalTable.size(); ++j) {
				if (result != "") {
					result += " ";
				}

				int selectID = selectIDsInFinalTable[j];

				if (_selectType[j] == TYPE_VARIABLE) {
					string varName = PKB::getPKBInstance()->getVarName(finalContent[i][selectID]);
					result += varName;
				}
				else if (_selectType[j] == TYPE_PROCEDURE) {
					string procName = PKB::getPKBInstance()->getProcName(finalContent[i][selectID]);
					result += procName;
				}
				else {
					result += to_string(finalContent[i][selectID]);
				}
			}

			resultList.push_back(result);
		}
	}

	resultList.sort();
	resultList.unique();
	logFinalResult(resultList);
	return resultList;
}

void QueryResultProjector::logFinalResult(list<string> resultStringList) {
	string str;
	for (auto &x : resultStringList) {
		str += x + " ";
	}
	SPALog::log(str);
}