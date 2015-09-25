#pragma once
#include <string>
#include <list>
//#include <cstdbool>
#include <vector>
#include <unordered_map>
#include "RelationshipTable.h"
#include "QueryTree.h"

using namespace std;

class QueryValidator {
private:
	unordered_map<string, string> varMap;
	RelationshipTable r;
	enum RETURN_TYPE { NONE, INVALID, VALID };
	QueryTree qt;

	//bool parseString(string query);
	bool isValidDeclaration(string declaration);
	bool isValidQuery(string query);
	bool checkRetVal(RETURN_TYPE retVal, bool &isFinished);
	bool isValidVariableName(string varName);
	RETURN_TYPE findSuchThatClause(string &subquery);
	RETURN_TYPE findSuchThatString(string &subquery);
	RETURN_TYPE findPatternClause(string &subquery);
	RETURN_TYPE parsePatternArg1(string relType, string &arg, string &varType);
	RETURN_TYPE parsePatternArg2(string relType, string &arg, string &varType);
	
	vector<string> split(string str, char c);
	vector<string> split(string str, char c, int num);
	string QueryValidator::trim(string content);
	//string QueryValidator::removeSpaces(string line);
	string stringToLower(string str);
	bool isVarNameExists(string varName);
	string getVarType(string varName);
	bool isStringVar(string str);
	bool isInteger(string str);
	bool isPositiveInteger(string str);
public:
	//void areValidQueries(list<string> queries);
	bool isValidDecAndQuery(string query);
	QueryTree getQueryTree(); 
};
#pragma once
