#include "QueryValidator.h"

using namespace std;

list<bool> QueryValidator::isValidQuery(list<string> queries)
{
	list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();

	for (iterQueries; iterQueries != queries.end(); iterQueries++) {
		isValid.push_back(true);
	}

	return isValid;
}

bool QueryValidator::parseQuery(string query)
{
	//char* queryChar = &query[0];
	vector<string> splitStr = split(query, ';');

	int size = splitStr.size();

	//declaration clauses
	for (int i = 0; i < size - 1; i++) {
		if (!parseDeclaration(splitStr.at(i))) {
		return false;
		}
	}

	return false;
}

bool QueryValidator::parseDeclaration(string declaration) {

	//vector<string> splitDec = split()
	return false;
}

//tokenizer
vector<string> QueryValidator::split(string str, char c) {
	vector<string> result;
	const char *strChar = str.c_str();

	do {
		const char *begin = strChar;

		while (*strChar != c && *strChar) {
			strChar++;
		}

		result.push_back(string(begin, strChar));
	} while (0 != *strChar++);

	return result;
}
