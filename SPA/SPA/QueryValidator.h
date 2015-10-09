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
	
	RETURN_TYPE findRel(string &subquery);
	bool parseRel(string &subquery);
	bool parseRelArgs(string relType, vector<string> &arrVar, vector<string> &varTypes);
	RETURN_TYPE findSuchThatString(string &subquery);
	
	RETURN_TYPE findPatternClause(string &subquery);
	bool parsePatternType(string word, string &relType, string &syn, string &synType);
	bool parsePatternArg1(string relType, string &arg, string &varType);
	bool parsePatternArg2(string relType, string &arg, string &varType);
	
	RETURN_TYPE findWithClause(string &subquery);
	bool parseWithNumber(string &subquery, string &relType, vector<string> &arrVar, vector<string> &varTypes);
	bool parseWithName(string &subquery, string &relType, vector<string> &arrVar, vector<string> &varTypes);

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
