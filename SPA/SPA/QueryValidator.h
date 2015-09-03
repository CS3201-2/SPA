#pragma once
#include <string>
#include <list>
//#include <cstdbool>
#include <vector>
#include <unordered_map>
//#include <QueryTree>

using namespace std;

class QueryValidator {
private:
	//QueryTree qt;
	unordered_map<string, string> varMap;
	bool parseString(string query);
	bool parseDeclaration(string declaration);
	vector<string> split(string str, char c);
	vector<string> QueryValidator::split(string str, char c, int num);
	bool isValidVariableName(string varName);
	string QueryValidator::trim(string content);
	string QueryValidator::removeSpaces(string line);
	bool parseQuery(string query);
	bool findSuchThat(string &subquery);
	string stringToLower(string str);
public:
	void areValidQueries(list<string> queries);
};
#pragma once
