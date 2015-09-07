#include "QueryValidator.h"
#include <iostream>
#include <regex>

/*void QueryValidator::areValidQueries(list<string> queries)
{
	//list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();

	for (iterQueries; iterQueries != queries.end(); iterQueries++) {
		//isValid.push_back(parseQuery(*iterQueries));
		if (!(*iterQueries).empty()) {
			//qt.start();
			varMap.clear();
			parseString(*iterQueries);
			//qt.end();
		}	
	}

}*/

bool QueryValidator::parseString(string query)
{
	vector<string> splitStr = split(trim(query), ';');

	if (splitStr.at(0).empty()) {
		//cout << "Invalid Query" << endl;
		//qt.invalid();
		return false;
	}

	int size = splitStr.size();
	//cout << size;
	int i = 0;
	//declaration clauses
	for (i; i < size - 1; i++) {
		if (!parseDeclaration(splitStr.at(i))) {
			//cout << "Invalid Query" << endl;
			//qt.invalid();
			return false;
		} 
	}

	if (!parseQuery(splitStr.at(i))) {
		//cout << "Invalid Query" << endl;
		//qt.invalid();
		return false;
	}

	//cout << "valid query";
	return true;
}

bool QueryValidator::parseDeclaration(string declaration) {

	vector<string> arrDec = split(declaration, ' ', 2);
	//cout << arrDec.at(1) << endl;

	if (!(arrDec.at(0).compare("stmt") == 0 || arrDec.at(0).compare("assign") == 0 ||
		arrDec.at(0).compare("while") == 0 || arrDec.at(0).compare("variable") == 0 ||
		arrDec.at(0).compare("constant") == 0 || arrDec.at(0).compare("prog_line") == 0)) {
		return false;
	}
		
		vector<string> synonyms = split(arrDec.at(1), ',');

		//cout << synonyms.at(0) << endl;

		for (int i = 0; i < synonyms.size(); i++) {
			
			//if (synonyms.at(i)) - check if empty (no variable after type)
			if (!isValidVariableName(synonyms.at(i))) {	
				return false;
			} else {
				varMap[synonyms.at(i)] = arrDec.at(0);
				//qt.addVariable(synonyms.at(i), arrDec.at(0));
				//cout << varMap.find("a")->second;
			}
		}
	
	
	return true;
}

bool QueryValidator::parseQuery(string query) {
	vector<string> arrClauses = split(query, ' ', 2); //don't split into 3 as will have tuples(multiple var) later
	
	if (stringToLower(arrClauses.at(0)).compare("select") != 0) {
		return false;
	}

	arrClauses = split(arrClauses.at(1), ' ', 2);

	if ( !(varNameExists(arrClauses.at(0)) || arrClauses.at(0).compare("_") == 0) ) {
		return false;
	}
	
	if (findSuchThatClause(arrClauses.at(1)) == INVALID) {
		//cout << "invalid such that";
		return false;
	}

	if (findPatternClause(arrClauses.at(1)) == INVALID) {
		//cout << "invalid pattern";
		return false;
	}

	if (findSuchThatClause(arrClauses.at(1)) == INVALID) {
		//cout << "invalid such that";
		return false;
	}
	
	if (arrClauses.at(1).size() > 1) {
		return false;
	}
	//cout << arrClauses.at(1).size();

	return true;
}

QueryValidator::RETURN_TYPE QueryValidator::findSuchThatClause(string &subquery){
	if (findSuchThatString(subquery) == NONE) {
		return NONE;
	}

	//cout << varMap.find("a")->second << endl;
	//cout << arrClauses.at(1) << endl;

	vector<string> arrClauses = split(subquery, '(', 2);
	
	if (!r.hasRelationship(stringToLower(arrClauses.at(0)))) {
		return INVALID;
	}

	string relType = stringToLower(arrClauses.at(0));

	if (arrClauses.at(1).find(")") == string::npos) {
		return INVALID;
	}

	arrClauses = split(arrClauses.at(1), ')', 2);

	vector<string> arrVar = split(arrClauses.at(0), ',');

	if (!r.isNumOfArgsEqual(relType, arrVar.size())) {
		return INVALID;
	}

	for (int i = 0; i < arrVar.size(); i++) {
		if (varNameExists(arrVar.at(i))) {
			r.isArgValid(relType, i + 1, getVarType(arrVar.at(i)));

		} else if (isStringVar(arrVar.at(i))) {
			r.isArgValid(relType, i + 1, "string");

		} else if ( isPositiveInteger(arrVar.at(i)) ) {
			r.isArgValid(relType, i + 1, "prog_line");

		} else if (arrVar.at(i).compare("_") == 0) {
			r.isArgValid(relType, i + 1, "all");

		} else {
			return INVALID;

		}
	}
	//cout << "yes";
	//qt.addRel(relType, arrVar);
	//cout << relType << " " << arrVar.at(0) << " " << arrVar.at(1) << endl;
	subquery = trim(arrClauses.at(1));
	return VALID;
}

QueryValidator::RETURN_TYPE QueryValidator::findPatternClause(string &subquery){
	vector<string> arrWords = split(subquery, ' ', 2);
	
	if (!(stringToLower(arrWords.at(0)).compare("pattern") == 0)) {
		return NONE;
	}

	string relType = arrWords.at(0);

	arrWords = split(arrWords.at(1), '(', 2);

	if (!(varNameExists(arrWords.at(0)) && getVarType(arrWords.at(0)).compare("assign") == 0)) {
		return INVALID;
	}

	if (arrWords.at(1).find(")") == string::npos) {
		return INVALID;
	}

	arrWords = split(arrWords.at(1), ')', 2);
	
	vector<string> arrVar = split(arrWords.at(0), ',');

	if (!r.isNumOfArgsEqual(relType, arrVar.size())) {
		return INVALID;
	}

	//arg1
	if (varNameExists(arrVar.at(0)) && getVarType(arrVar.at(0)).compare("assign") == 0) {
		r.isArgValid(relType, 1, getVarType(arrVar.at(0)));

	} else if (isStringVar(arrVar.at(0))) {
		r.isArgValid(relType, 1, "string");

	} else if (arrVar.at(0).compare("_") == 0) {
		r.isArgValid(relType, 1, "all");

	} else {
		return INVALID;

	}
	
	//arg2
	string arg2 = arrVar.at(1);
	string value;
	if (arg2.size() >= 5) {
		if (arg2.at(0) == '_' && arg2.at(arg2.size() - 1) == '_'  &&
			arg2.at(1) == '\"' && arg2.at(arg2.size() - 2) == '\"') {
			
			value = arg2.substr(2, arg2.size() - 4);

			if (isValidVariableName(value)) {
				r.isArgValid(relType, 2, "variable");

			} else if (isInteger(value)) {
				r.isArgValid(relType, 2, "constant");

			}
		}

	} else if (arg2.compare("_") == 0) {
		r.isArgValid(relType, 1, "all");
	} else {
		return INVALID;
	}

	//qt.addPattern(arrVar.at(0), arrVar.at(1));
	//or qt.addRel(reltype, arrVar);
	//cout << "pattern " << arrVar.at(0) << " " << value << endl;
	subquery = trim(arrWords.at(1));
	
	return VALID;
}

QueryValidator::RETURN_TYPE QueryValidator::findSuchThatString(string &subquery) {
	vector<string> arrWords = split(subquery, ' ', 3);

	if (stringToLower(arrWords.at(0)).compare("such") == 0 && 
			stringToLower(arrWords.at(1)).compare("that") == 0) {
		subquery = arrWords.at(2);
		return VALID;
	}

	
	//cout << arrWords.at(2);
	/*if (stringToLower(arrWords.at(0)).compare("such") == 0) {
		arrWords = split(arrWords.at(1), ' ', 2);

		if (!(stringToLower(arrWords.at(0)).compare("that") == 0)) {
			return false;
		} else {
			subquery = arrWords.at(1);
		}
	}*/

	return NONE;
}

bool QueryValidator::isValidVariableName(string varName)
{
	//cout << "no";
	if (varName.length() == 0) {
		return false;
	}
	
	for (int i = 0; i < varName.length(); i++) {
		if (i == 0) {
			if (!isalpha(varName.at(i))) {
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
	line.erase(0, line.find_first_not_of(' '));       
	line.erase(line.find_last_not_of(' ') + 1);
	line.erase(remove(line.begin(), line.end(), '\n'), line.end());
	line.erase(remove(line.begin(), line.end(), '\t'), line.end());
	return regex_replace(line, regex("[' ']{2,}"), " ");
}

/*string QueryValidator::removeSpaces(string line) {
	return regex_replace(line, regex("[' ']{1,}"), "");
}*/

string QueryValidator::stringToLower(string str) {
	transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

bool QueryValidator::varNameExists(string varName){
	auto it = varMap.find(varName);
	
	if (it == varMap.end()) { //variable not found
		return false;
	}

	return true;
}

string QueryValidator::getVarType(string varName) {
	return varMap.find(varName)->second;
}

bool QueryValidator::isStringVar(string str) {
	if (!(str.front() == '\"' && str.back() == '\"')) {
		return false;
	}

	string varName = str.substr(1, str.size() - 2);

	return isValidVariableName(varName);
}

bool QueryValidator::isInteger(string str) {
	if (str.empty() || ((!isdigit(str.at(0))) && (str.at(0) != '-') &&
			(str.at(0) != '+'))) {
		return false;
	}

	char *p;
	strtol(str.c_str(), &p, 10);

	return (*p == 0);
}

bool QueryValidator::isPositiveInteger(string str) {
	if ( str.empty() || !isdigit(str.at(0)) ) {
		return false;
	}

	char *p;
	strtol(str.c_str(), &p, 10);

	return (*p == 0);
}
