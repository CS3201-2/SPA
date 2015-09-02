#include "QueryValidator.h"
#include <iostream>
#include <regex>

using namespace std;

void QueryValidator::areValidQueries(list<string> queries)
{
	//list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();

	for (iterQueries; iterQueries != queries.end(); iterQueries++) {
		//isValid.push_back(parseQuery(*iterQueries));
		if (!(*iterQueries).empty()) {
			//qt.start();
			parseString(*iterQueries);
			//qt.end();
		}
		
	}

}

bool QueryValidator::parseString(string query)
{
	//char* queryChar = &query[0];
	vector<string> splitStr = split(query, ';');

	if (splitStr.at(0).empty()) {
		cout << "Invalid Query" << endl;
		return false;
	}

	int size = splitStr.size();
	//cout << size;
	int i = 0;
	//declaration clauses
	for (i; i < size - 1; i++) {
		//cout << "called";
		if (!parseDeclaration(splitStr.at(i))) {
			//cout << "\n exiting";
			cout << "Invalid Query" << endl;
			return false;
		} 
	}

	if (!parseQuery(splitStr.at(i))) {
		return false;
	}
	//cout << i;
	return true;
}

bool QueryValidator::parseDeclaration(string declaration) {

	vector<string> arrDec = split(declaration, ' ', 2);
	//cout << arrDec.at(1) << endl;

	if (arrDec.at(0).compare("stmt") == 0 || arrDec.at(0).compare("assign") == 0 ||
		arrDec.at(0).compare("while") == 0 || arrDec.at(0).compare("variable") == 0 ||
		arrDec.at(0).compare("constant") == 0 || arrDec.at(0).compare("prog_line") == 0) {
		
		vector<string> synonyms = split(removeSpaces(arrDec.at(1)), ',');

		//cout << synonyms.at(0) << endl;

		for (int i = 0; i < synonyms.size(); i++) {
			//if (synonyms.at(i)) - check if empty (no variable after type)
			if (!isValidVariableName(synonyms.at(i))) {	
				return false;
			} else {
				varMap[synonyms.at(i)] = arrDec.at(0);
				//qt.addVariable(synonyms.at(i), arrDec.at(0));
				//cout << varMap.find(synonyms.at(i))->second;
			}
		}
	}
	
	return true;
}

bool QueryValidator::parseQuery(string query) {
	vector<string> arrClauses = split(query, ' ', 2); //don't split into 3 as will have tuples(multiple var) later.
	
	transform(arrClauses.at(0).begin(), arrClauses.at(0).end(), 
		arrClauses.at(0).begin(), tolower);

	if (arrClauses.at(0).compare("select") != 0) {
		return false;
	}

	arrClauses = split(arrClauses.at(1), ' ', 2);

	auto it = varMap.find(arrClauses.at(1));
	if (it == varMap.end()) { //key not found
		return false;
	}

	//cout << varMap.find("a")->second << endl;
	//cout << arrClauses.at(1) << endl;
	return true;
}

bool QueryValidator::isValidVariableName(string varName)
{
	//cout << "no";
	if (varName.length() == 0) {
		return false;
	}
	
	for (int i = 0; i < varName.length(); i++) {
		//cout << "\n isValidVariableName";
		if (i == 0) {
			if (!isalpha(varName.at(i))) {
				//cout << "yes";
				return false;
			}
		} else if (!(isalnum(varName.at(i)) || varName.at(i) == '#')) {
			return false;
		}
	}

	//cout << varName << endl;
	return true;
}

//tokenizer
vector<string> QueryValidator::split(string str, char c) {
	vector<string> result;
	const char *strChar = str.c_str();

	do {
		//cout << "\n split";
		const char *begin = strChar;

		while (*strChar != c && *strChar) {
			strChar++;
		}

		result.push_back(trim(string(begin, strChar)));
	} while (0 != *strChar++);

	return result;
}

vector<string> QueryValidator::split(string str, char c, int num) {
	vector<string> result;
	const char *strChar = str.c_str();
	int i = 1;
	
	do {
		//cout << "\n split";
		const char *begin = strChar;

		while (*strChar != c && *strChar) {
			strChar++;
		}

		result.push_back(trim(string(begin, strChar)));
	} while (0 != *strChar++ && ++i < num);

	if (i == num) {
		const char *begin = strChar;

		while (0 != *strChar++  && *strChar) {
			strChar++;
		}

		result.push_back(trim(string(begin, strChar)));
	}

	return result;
}

string QueryValidator::trim(string line) {
	//cout << "\n trim";
	line.erase(0, line.find_first_not_of(' '));       
	line.erase(line.find_last_not_of(' ') + 1);
	return regex_replace(line, regex("[' ']{2,}"), " ");
}

string QueryValidator::removeSpaces(string line) {
	return regex_replace(line, regex("[' ']{1,}"), "");
}
