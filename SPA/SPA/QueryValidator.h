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
	QueryTree qt;

	bool isValidDeclaration(string declaration);
	bool isValidQuery(string query);
	
	bool parseRel(string &subquery);
	bool parseRelArgs(string relType, vector<string> &arrVar, vector<string> &varTypes);
	
	bool findPatternClause(string &subquery);
	bool parsePatternType(string word, string &relType, string &syn, string &synType);
	bool parsePatternArg1(string relType, string &arg, string &varType);
	bool parsePatternArg2(string relType, string &arg, string &varType);
	
	bool findWithClause(string &subquery);
	bool parseWithNumber(string &subquery, string &relType, vector<string> &arrVar, vector<string> &varTypes);
	bool parseWithName(string &subquery, string &relType, vector<string> &arrVar, vector<string> &varTypes);

	vector<string> split(string str, char c);
	vector<string> split(string str, char c, int num);
	string QueryValidator::trim(string content);
	string QueryValidator::removeSpaces(string line);
	string stringToLower(string str);
	bool isVarNameExists(string varName);
	
	bool isValidVariableName(string varName);
	string getVarType(string varName);
	bool isStringVar(string str);
	bool isInteger(string str);
	bool isPositiveInteger(string str);
	vector<string> parseExpression(string expression);
	bool isValidExpression(string expression);
	
	bool isOperator(string str);
	bool isParenthesis(string str); 
	int countNumOfLeftParenthesis(string stmt);
	int countNumOfRightParenthesis(string stmt);

public:
	bool isValidDecAndQuery(string query);
	QueryTree getQueryTree(); 
};
#pragma once
