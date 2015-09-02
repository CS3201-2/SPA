#pragma once
#include <string>
#include <list>
#include <cstdbool>
#include <vector>
#include <map>
//#include <QueryTree>

using namespace std;

class QueryValidator {
private:
	//QueryTree qt;
	map<string, string> varMap;
	bool parseQuery(string query);
	bool parseDeclaration(string declaration);
	vector<string> split(string str, char c);
	vector<string> QueryValidator::split(string str, char c, int num);
	bool isValidVariableName(string varName);
	string QueryValidator::trim(string content);
	string QueryValidator::removeSpaces(string line);
public:
	void areValidQueries(list<string> queries);
};
#pragma once
