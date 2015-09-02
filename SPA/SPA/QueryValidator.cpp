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
			parseQuery(*iterQueries);
			//qt.end();
		}
		
	}

}

bool QueryValidator::parseQuery(string query)
{
	//char* queryChar = &query[0];
	vector<string> splitStr = split(query, ';');

	if (splitStr.at(0).empty()) {
		cout << "Invalid Query" << endl;
		return false;
	}

	int size = splitStr.size();
	int i;
	//cout << size;
	//declaration clauses
	for (i = 0; i < size - 1; i++) {
		//cout << "called";
		if (!parseDeclaration(splitStr.at(i))) {
			//cout << "\n exiting";
			cout << "Invalid Query" << endl;
			return false;
		} 
	}
	//cout << i;
	return true;
}

bool QueryValidator::parseDeclaration(string declaration) {

	vector<string> arrDec = split(declaration, ' ');

	if (arrDec.at(0).compare("stmt") == 0 || arrDec.at(0).compare("assign") == 0 ||
		arrDec.at(0).compare("while") == 0 || arrDec.at(0).compare("variable") == 0 ||
		arrDec.at(0).compare("constant") == 0 || arrDec.at(0).compare("prog_line") == 0) {
		vector<string> synonyms = split(arrDec.at(1), ';');

		for (int i = 0; i < synonyms.size(); i++) {
			
			//if (synonyms.at(i)) - check if empty (no variable after type)
			if (!isValidVariableName(synonyms.at(i))) {	
				return false;
			} else {
				//qt.addVariable(arrDec.at(0), synonyms.at(i));
				//cout << "\n parseDeclaration";
			}
		}
	}
	
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

	cout << varName << endl;
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
		//cout << trim(string(begin, strChar)) << "\n";
		result.push_back(trim(string(begin, strChar)));
		//cout << string(begin, strChar) << "\n";
	} while (0 != *strChar++);

	return result;
}

string QueryValidator::trim(string line) {
	//cout << "\n trim";
	line.erase(0, line.find_first_not_of(' '));       
	line.erase(line.find_last_not_of(' ') + 1);
	return regex_replace(line, regex("[' ']{2,}"), " ");
}
