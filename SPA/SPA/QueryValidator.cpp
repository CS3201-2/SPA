#include "QueryValidator.h"
#include <cstdbool>

using namespace std;

list<bool> QueryValidator::isValidQuery(list<string> queries)
{
	list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();

	for (iterQueries ; iterQueries != queries.end(); iterQueries++) {
			isValid.push_back(true);
	}

	return isValid;
}

bool QueryValidator::parseQuery(string query)
{
	char* queryChar = &query[0];
	string splitStr = strtok(queryChar, ";");

	int size = sizeof(splitStr);

	//declaration clauses
	for (int i = 0; i < size - 1; i++) {
		if (!parseDeclaration(splitStr[i]))
			break;
	}
	
	return false;
}

bool QueryValidator::parseDeclaration(string declaration)
{
	return false;
}
