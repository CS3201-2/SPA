#include "QueryValidator.h"
#include <iostream>
#include <regex>

using namespace std;

list<bool> QueryValidator::isValidQuery(list<string> queries)
{
	list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();

	for (iterQueries; iterQueries != queries.end(); iterQueries++) {
		isValid.push_back(parseQuery(*iterQueries));
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
		/*if (!parseDeclaration(splitStr.at(i))) {
		return false;
		} */
	}

	return false;
}

bool QueryValidator::parseDeclaration(string declaration) {

	vector<string> arrDec = split(declaration, ' ');

	if (arrDec.at(0).compare("stmt") == 0 || arrDec.at(0).compare("assign") == 0 ||
		arrDec.at(0).compare("while") == 0 || arrDec.at(0).compare("variable") == 0 ||
		arrDec.at(0).compare("constant") == 0 || arrDec.at(0).compare("prog_line") == 0) {
		vector<string> synonyms = split(arrDec.at(1), ';');

		for (int i = 0; i < synonyms.size(); i++) {
			//if (synonyms.at(i)) - check if empty (no variable after type)
			if (isValidVariableName(synonyms.at(i))) {
				//qt.addVariable(arrDec.at(0), synonyms.at(i));
			}
			else {
				return false;
			}
		}
	}

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
		cout << trim(string(begin, strChar)) << "\n";
		result.push_back(string(begin, strChar));
		//cout << string(begin, strChar) << "\n";
	} while (0 != *strChar++);

	return result;
}

string QueryValidator::trim(string line) {
	line.erase(0, line.find_first_not_of(' '));       //prefixing spaces
	line.erase(line.find_last_not_of(' ') + 1);
	return regex_replace(line, regex("[' ']{2,}"), " ");
}

bool QueryValidator::isValidVariableName(string varName)
{

	return false;
}
