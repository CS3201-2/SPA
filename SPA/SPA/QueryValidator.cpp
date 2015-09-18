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

bool QueryValidator::isValidDecAndQuery(string query)
{
	vector<string> splitStr = split(trim(query), ';');
	//cout << "isValidDecAndQuery";
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
		if (!isValidDeclaration(splitStr.at(i))) {
			//cout << "Invalid Query" << endl;
			//qt.invalid();
			return false;
		} 
	}

	if (!isValidQuery(splitStr.at(i))) {
		//cout << "Invalid Query" << endl;
		//qt.invalid();
		return false;
	}

	//cout << "valid query";
	return true;
}

QueryTree QueryValidator::getQueryTree() {
	return qt;
}

bool QueryValidator::isValidDeclaration(string declaration) {
	//cout << "isValidDeclaration" << endl;
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
				//cout << "dec: " << synonyms.at(i) <<" "<< arrDec.at(0) << endl;
				qt.insertVariable(synonyms.at(i), arrDec.at(0));
				//cout << varMap.find("a")->second;
			}
		}
	
	
	return true;
}

bool QueryValidator::isValidQuery(string query) {
	//cout << "isValidQuery";
	vector<string> arrClauses = split(query, ' ', 2); //don't split into 3 as will have tuples(multiple var) later
	
	if (stringToLower(arrClauses.at(0)).compare("select") != 0) {
		return false;
	}

	arrClauses = split(arrClauses.at(1), ' ', 2);

	if ( !(isVarNameExists(arrClauses.at(0)))) {// || arrClauses.at(0).compare("_") == 0) ) {
		return false;
	}	

	qt.insertSelect(arrClauses.at(0), getVarType(arrClauses.at(0)));  

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
	//cout << "findSuchThat" << endl;
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

	vector<string> varTypes(arrVar.size());
	//cout << "\nsuch that: parsing args";
	for (int i = 0; i < arrVar.size(); i++) {
		if (isVarNameExists(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, getVarType(arrVar.at(i)))) {
				return INVALID;
			} else {
				//for iter 1 only
				if (getVarType(arrVar.at(i)) == "stmt") {
					varTypes.at(i) = "all";
				} else {
					varTypes.at(i) = getVarType(arrVar.at(i));
				}
			}

		} else if (isStringVar(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, "string")) {
				return INVALID;
			} else {
				arrVar.at(i) = arrVar.at(i).substr(1, arrVar.at(i).size() - 2);
				varTypes.at(i) = "string";
			}

		} else if ( isPositiveInteger(arrVar.at(i)) ) {
			if (!r.isArgValid(relType, i + 1, "prog_line")) {
				return INVALID;
			} else {
				varTypes.at(i) = "prog_line";
			}

		} else if (arrVar.at(i).compare("_") == 0) {
			if (!r.isArgValid(relType, i + 1, "all")) {
				return INVALID;
			} else {
				varTypes.at(i) = "all";
			}

		} else {
			return INVALID;

		}
	}

	qt.insertSuchThat(relType, arrVar, varTypes);

	cout << "such that: " << relType << " " << arrVar.at(0) << " " << arrVar.at(1) << endl;
	//cout << varTypes.at(0) << " " << varTypes.at(1) << endl;
	subquery = trim(arrClauses.at(1));
	return VALID;
}

QueryValidator::RETURN_TYPE QueryValidator::findPatternClause(string &subquery){
	vector<string> arrWords = split(subquery, ' ', 2);
	//cout << "findPattern" << endl;
	if (!(stringToLower(arrWords.at(0)).compare("pattern") == 0)) {
		return NONE;
	}

	string relType = stringToLower(arrWords.at(0));

	arrWords = split(arrWords.at(1), '(', 2);

	if (!(isVarNameExists(arrWords.at(0)) && getVarType(arrWords.at(0)).compare("assign") == 0)) {
		return INVALID;
	}

	string syn = arrWords.at(0), synType = "assign";

	if (arrWords.at(1).find(")") == string::npos) {
		return INVALID;
	}

	arrWords = split(arrWords.at(1), ')', 2);
	
	vector<string> arrVar = split(arrWords.at(0), ',');

	if (!r.isNumOfArgsEqual(relType, arrVar.size())) {
		return INVALID;
	}

	vector<string> varType(arrVar.size());
	//arg1
	if (isVarNameExists(arrVar.at(0)) && getVarType(arrVar.at(0)).compare("assign") == 0) {
		if (!r.isArgValid(relType, 1, getVarType(arrVar.at(0)))) {
			return INVALID;
		} else {
			varType.at(0) = "assign";
		}

	} else if (isStringVar(arrVar.at(0))) {
		if (!r.isArgValid(relType, 1, "string")) {
			return INVALID;
		} else {
			arrVar.at(0) = arrVar.at(0).substr(1, arrVar.at(0).size() - 2);
			varType.at(0) = "string";
		}

	} else if (arrVar.at(0).compare("_") == 0) {
		if (!r.isArgValid(relType, 1, "all")) {
			return INVALID;
		} else {
			varType.at(0) = "all";
		}

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
				if (!r.isArgValid(relType, 2, "variable")) {
					return INVALID;
				} else {
					varType.at(1) = "variable";
				}

			} else if (isInteger(value)) {
				if (!r.isArgValid(relType, 2, "constant")) {
					return INVALID;
				}
				else {
					varType.at(1) = "constant";
				}

			}
		}

	} else if (arg2.at(0) == '\"' && arg2.at(arg2.size() - 1) == '\"'){
		value = arg2.substr(1, arg2.size() - 2);
		
		if (isValidVariableName(value)) {
			if (!r.isArgValid(relType, 2, "variable")) {
				return INVALID;
			}
			else {
				varType.at(1) = "variable";
			}
		} else if (isInteger(value)) {
			if (!r.isArgValid(relType, 2, "constant")) {
				return INVALID;
			} else {
				varType.at(1) = "constant";
			}
		} 
	} else if (arg2.compare("_") == 0) {
		r.isArgValid(relType, 1, "all");
	} else {
		return INVALID;
	}

	qt.insertPattern(syn, synType, arrVar, varType);

	//or qt.addRel(reltype, arrVar);
	//cout << "pattern " << arrVar.at(0) << " " << value << endl;
	subquery = trim(arrWords.at(1));
	
	return VALID;
}

QueryValidator::RETURN_TYPE QueryValidator::findSuchThatString(string &subquery) {
	vector<string> arrWords = split(subquery, ' ', 3);
	//cout << "findSuchThatString" << endl;
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

bool QueryValidator::isVarNameExists(string varName){
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
