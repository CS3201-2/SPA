#pragma once
#include <string>
#include <list>
#include <cstdbool>
#include <vector>
//#include <QueryTree>

using namespace std;

class QueryValidator {
private:
	//QueryTree qt;
	bool parseQuery(string query);
	bool parseDeclaration(string declaration);
	vector<string> split(string str, char c);
	bool isValidVariableName(string varName);
	string QueryValidator::trim(string content);
public:
	void areValidQueries(list<string> queries);
};
#pragma once
