#pragma once
#include <string>
#include <list>
//#include <cstdbool>
#include <vector>
#include <unordered_map>
#include "RelationshipTable.h"
//#include <QueryTree>

using namespace std;

class QueryValidator {
private:
	unordered_map<string, string> varMap;
	RelationshipTable r;
	enum RETURN_TYPE { NONE, INVALID, VALID };
	//QueryTree qt;

	bool parseString(string query);
	bool parseDeclaration(string declaration);
	bool parseQuery(string query);
	//bool parseRel()
	//bool parsePattern()
	bool isValidVariableName(string varName);
	RETURN_TYPE findSuchThatClause(string &subquery);
	RETURN_TYPE findPatternClause(string &subquery);
	RETURN_TYPE findSuchThatString(string &subquery);
	
	vector<string> split(string str, char c);
	vector<string> split(string str, char c, int num);
	string QueryValidator::trim(string content);
	//string QueryValidator::removeSpaces(string line);
	string stringToLower(string str);
	bool varNameExists(string varName);
	string getVarType(string varName);
	bool isStringVar(string str);
	bool isInteger(string str);
public:
	void areValidQueries(list<string> queries);
};
#pragma once
