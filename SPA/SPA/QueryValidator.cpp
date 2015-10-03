#include "QueryValidator.h"
#include <iostream>
#include <regex>

const char SYMBOL_SEMICOLON = ';';
const char SYMBOL_SPACE = ' ';
const char SYMBOL_COMMA = ',';
//const char SYMBOL_UNDERSCORE = '-';
//const char SYMBOL_QUOTE = '\"';
const char SYMBOL_OPEN_BRACKET = '(';
const char SYMBOL_CLOSE_BRACKET = ')';

const vector<string> KEYWORDS = { "constant", "stmt", "assign", "while", "if", 
	"procedure", "call", "prog_line", "select", "modifies", "uses", "parent", 
	"parent*", "follows", "follows*", "pattern", "next", "next*", "calls", 
	"calls*", "affects", "affects*", "with" };

const string VARTYPE_VARIABLE = "variable";
const string VARTYPE_STRING = "string";
const string VARTYPE_SUBSTRING = "substring";
const string VARTYPE_CONSTANT = "constant";
const string VARTYPE_STMT = "stmt";
const string VARTYPE_ASSIGN = "assign";
const string VARTYPE_WHILE = "while";
const string VARTYPE_IF = "if";
const string VARTYPE_PROCEDURE = "procedure";
const string VARTYPE_PROC_NAME = "proc_name";
const string VARTYPE_CALL = "call";
const string VARTYPE_PROG_LINE = "prog_line";
const string VARTYPE_NUMBER = "number";
const string VARTYPE_ALL = "all";

bool QueryValidator::isValidDecAndQuery(string query)
{
	vector<string> splitStr = split(trim(query), SYMBOL_SEMICOLON);
	//cout << "isValidDecAndQuery";
	if (splitStr.at(0).empty()) {
		cout << "Invalid Query" << endl;
		//qt.invalid();
		return false;
	}

	int size = splitStr.size();
	//cout << size;
	int i = 0;
	//declaration clauses
	for (i; i < size - 1; i++) {
		if (!isValidDeclaration(splitStr.at(i))) {
			cout << "Invalid Query" << endl;
			//qt.invalid();
			return false;
		}
	}

	if (!isValidQuery(splitStr.at(i))) {
		cout << "Invalid Query" << endl;
		//qt.invalid();
		return false;
	}

	cout << "valid query";
	return true;
}

QueryTree QueryValidator::getQueryTree() {
	return qt;
}

bool QueryValidator::isValidDeclaration(string declaration) {
	//cout << "isValidDeclaration" << endl;
	vector<string> arrDec = split(declaration, SYMBOL_SPACE, 2);
	//cout << arrDec.at(1) << endl;

	if (!(arrDec.at(0).compare(VARTYPE_STMT) == 0 || arrDec.at(0).compare(VARTYPE_ASSIGN) == 0 ||
		arrDec.at(0).compare(VARTYPE_WHILE) == 0 || arrDec.at(0).compare(VARTYPE_VARIABLE) == 0 ||
		arrDec.at(0).compare(VARTYPE_CONSTANT) == 0 || arrDec.at(0).compare(VARTYPE_PROG_LINE) == 0 ||
		arrDec.at(0).compare(VARTYPE_IF) == 0 || arrDec.at(0).compare(VARTYPE_PROCEDURE) == 0 ||
		arrDec.at(0).compare(VARTYPE_CALL) == 0)) {
		return false;
	}

	vector<string> synonyms = split(arrDec.at(1), SYMBOL_COMMA);

	//cout << synonyms.at(0) << endl;

	for (int i = 0; i < synonyms.size(); i++) {

		//if (synonyms.at(i)) - check if empty (no variable after type)
		if (!isValidVariableName(synonyms.at(i)) || isVarNameExists(synonyms.at(i))) {
			return false;
		}
		else {
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
	vector<string> arrClauses = split(query, SYMBOL_SPACE, 2); //don't split into 3 as will have tuples(multiple var) later

	if (stringToLower(arrClauses.at(0)).compare("select") != 0) {
		return false;
	}

	arrClauses = split(arrClauses.at(1), SYMBOL_SPACE, 2);

	if (!(isVarNameExists(arrClauses.at(0)))) {// || arrClauses.at(0).compare("_") == 0) ) {
		return false;
	}

	qt.insertSelect(arrClauses.at(0), getVarType(arrClauses.at(0)));

	bool isFinished = false;
	RETURN_TYPE retVal;

	while (!isFinished) {
		retVal = findSuchThatClause(arrClauses.at(1));
		
		if (checkRetVal(retVal, isFinished) == false) {
			return false;
		}

		retVal = findPatternClause(arrClauses.at(1));

		if (checkRetVal(retVal, isFinished) == false) {
			return false;
		}

		/*retVal = findWithClause(arrClauses.at(1));

		if (checkRetVal(retVal, isFinished) == false) {
			return false;
		}*/	
	}

	//cout << arrClauses.at(1).size();	
	return true;
}

bool QueryValidator::checkRetVal(RETURN_TYPE retVal, bool & isFinished){
	if (retVal == INVALID) {
		return false;
	} else if (retVal == NONE) {
		isFinished = true;
		return true;
	} else if (retVal == VALID) {
		isFinished = false;
		return true;
	}

	return false;
}

QueryValidator::RETURN_TYPE QueryValidator::findSuchThatClause(string &subquery) {
	//cout << "findSuchThat" << endl;
	if (findSuchThatString(subquery) == NONE) {
		return NONE;
	}

	//cout << varMap.find("a")->second << endl;
	//cout << arrClauses.at(1) << endl;

	vector<string> arrClauses = split(subquery, SYMBOL_OPEN_BRACKET, 2);

	if (!r.hasRelationship(stringToLower(arrClauses.at(0)))) {
		return INVALID;
	}

	string relType = stringToLower(arrClauses.at(0));

	if (arrClauses.at(1).find(")") == string::npos) {
		return INVALID;
	}

	arrClauses = split(arrClauses.at(1), SYMBOL_CLOSE_BRACKET, 2);

	vector<string> arrVar = split(arrClauses.at(0), SYMBOL_COMMA);

	if (!r.isNumOfArgsEqual(relType, arrVar.size())) {
		return INVALID;
	}

	vector<string> varTypes(arrVar.size());
	//cout << "\nsuch that: parsing args";
	if (parseSuchThatArgs(relType, arrVar, varTypes) == INVALID) {
		return INVALID;
	}
	
	qt.insertSuchThat(relType, arrVar, varTypes);

	cout << "such that: " << relType << " " << arrVar.at(0) << " " << arrVar.at(1) << endl;
	//cout << varTypes.at(0) << " " << varTypes.at(1) << endl;
	subquery = trim(arrClauses.at(1));
	return VALID;
}

QueryValidator::RETURN_TYPE QueryValidator::parseSuchThatArgs(string relType, 
		vector<string>& arrVar, vector<string>& varTypes){
	
	for (int i = 0; i < arrVar.size(); i++) {
		if (isVarNameExists(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, getVarType(arrVar.at(i)))) {
				return INVALID;
			} else {
				varTypes.at(i) = getVarType(arrVar.at(i));
			}

		} else if (isStringVar(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, VARTYPE_STRING)) {
				return INVALID;
			} else {
				arrVar.at(i) = arrVar.at(i).substr(1, arrVar.at(i).size() - 2);
				varTypes.at(i) = VARTYPE_STRING;
			}

		} else if (isPositiveInteger(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, VARTYPE_PROG_LINE)) {
				return INVALID;
			} else {
				varTypes.at(i) = VARTYPE_PROG_LINE;
			}

		} else if (arrVar.at(i).compare("_") == 0) {
			if (!r.isArgValid(relType, i + 1, VARTYPE_ALL)) {
				return INVALID;
			} else {
				varTypes.at(i) = VARTYPE_ALL;
			}

		} else {
			return INVALID;
		}
	}
<<<<<<< HEAD
=======

	qt.insertSuchThat(relType, arrVar, varTypes);

	//cout << "such that: " << relType << " " << arrVar.at(0) << " " << arrVar.at(1) << endl;
	//cout << varTypes.at(0) << " " << varTypes.at(1) << endl;
	subquery = trim(arrClauses.at(1));
	return VALID;
>>>>>>> master
}

QueryValidator::RETURN_TYPE QueryValidator::findPatternClause(string &subquery) {
	vector<string> arrWords = split(subquery, SYMBOL_SPACE, 2);
	//cout << "findPattern" << endl;
	if (!(stringToLower(arrWords.at(0)).compare("pattern") == 0)) {
		return NONE;
	}

	arrWords = split(arrWords.at(1), SYMBOL_OPEN_BRACKET, 2);
	
	if (isVarNameExists(arrWords.at(0))) {
		if (getVarType(arrWords.at(0)).compare(VARTYPE_ASSIGN) == 0 ||
			getVarType(arrWords.at(0)).compare(VARTYPE_WHILE) == 0 ||
			getVarType(arrWords.at(0)).compare(VARTYPE_IF) == 0) {

			string relType, syn, synType;
			
			if (getVarType(arrWords.at(0)).compare(VARTYPE_ASSIGN) == 0) {
				relType = "patternAssign";
				syn = arrWords.at(0);
				synType = VARTYPE_ASSIGN;

			} else if (getVarType(arrWords.at(0)).compare(VARTYPE_WHILE) == 0) {
				relType = "patternWhile";
				syn = arrWords.at(0);
				synType = VARTYPE_WHILE;

			} else if (getVarType(arrWords.at(0)).compare(VARTYPE_IF) == 0) {
				relType = "patternIf";
				syn = arrWords.at(0);
				synType = VARTYPE_IF;

			} else {
				return INVALID;
			}
			
			if (arrWords.at(1).find(")") == string::npos) {
				return INVALID;
			}
			
			arrWords = split(arrWords.at(1), SYMBOL_CLOSE_BRACKET, 2);

			vector<string> arrVar = split(arrWords.at(0), SYMBOL_COMMA);

			if (!r.isNumOfArgsEqual(relType, arrVar.size())) {
				return INVALID;
			}
			
			vector<string> varType(arrVar.size());
			//arg1
			if (parsePatternArg1(relType, arrVar.at(0), varType.at(0)) == INVALID) {
				return INVALID;
			}
			
			//arg2
			if (parsePatternArg2(relType, arrVar.at(1), varType.at(1)) == INVALID) {
				return INVALID;
			}

			qt.insertPattern(syn, synType, arrVar, varType);
			cout << "pattern " << arrVar.at(0) << " " << arrVar.at(1) << endl;
			cout << "pattern " << varType.at(0) << " " << varType.at(1) << endl;
		} 
	} else {
		return INVALID;
	}
	//cout << "pattern " << arrVar.at(0) << " " << value << endl;
	subquery = trim(arrWords.at(1));

	return VALID;
}

QueryValidator::RETURN_TYPE QueryValidator::parsePatternArg1(string relType, 
		string &arg, string &varType) {
	
	if (isVarNameExists(arg) && getVarType(arg).compare(VARTYPE_ASSIGN) == 0) {
		if (!r.isArgValid(relType, 1, getVarType(arg))) {
			return INVALID;
		} else {
			varType = VARTYPE_ASSIGN;
		}

	} else if (isStringVar(arg)) {
		if (!r.isArgValid(relType, 1, VARTYPE_STRING)) {
			return INVALID;
		} else {
			arg = arg.substr(1, arg.size() - 2);
			varType = VARTYPE_STRING;
		}

	} else if (arg.compare("_") == 0) {
		if (!r.isArgValid(relType, 1, VARTYPE_ALL)) {
			return INVALID;
		} else {
			varType = VARTYPE_ALL;
		}

	} else {
		return INVALID;
	}
}

QueryValidator::RETURN_TYPE QueryValidator::parsePatternArg2(string relType, 
		string &arg, string &varType) {
	
	//string value;
	if (arg.size() >= 5) {
		if (arg.at(0) == '_' && arg.at(arg.size() - 1) == '_'  &&
			arg.at(1) == '\"' && arg.at(arg.size() - 2) == '\"') {

			arg = arg.substr(2, arg.size() - 4);

			if (isValidVariableName(arg)) {
				if (!r.isArgValid(relType, 2, VARTYPE_SUBSTRING)) {
					return INVALID;
				} else {
					varType = VARTYPE_SUBSTRING;
				}

			} else if (isInteger(arg)) {
				if (!r.isArgValid(relType, 2, VARTYPE_SUBSTRING)) {
					return INVALID;
				} else {
					varType = VARTYPE_SUBSTRING;
				}
			}
		}

	} else if (arg.at(0) == '\"' && arg.at(arg.size() - 1) == '\"') {
		arg = arg.substr(1, arg.size() - 2);

		if (isValidVariableName(arg)) {
			if (!r.isArgValid(relType, 2, VARTYPE_STRING)) {
				return INVALID;
			} else {
				varType = VARTYPE_STRING;
			}
		} else if (isInteger(arg)) {
			if (!r.isArgValid(relType, 2, VARTYPE_STRING)) {
				return INVALID;
			} else {
				varType = VARTYPE_STRING;
			}
<<<<<<< HEAD
		}
	} else if (arg.compare("_") == 0) {
		if (!r.isArgValid(relType, 2, VARTYPE_ALL)) {
			return INVALID;
		} else {
			varType = VARTYPE_ALL;
=======
		} 
	} else if (arg2.compare("_") == 0) {
		if (!r.isArgValid(relType, 2, "all")) {
			return INVALID;
		}
		else {
			varType.at(1) = "all";
>>>>>>> master
		}
	} else {
		return INVALID;
	}
<<<<<<< HEAD
=======

	qt.insertPattern(syn, synType, arrVar, varType);

	//or qt.addRel(reltype, arrVar);
	//cout << "pattern " << arrVar.at(0) << " " << value << endl;
	//cout << "pattern " << varType.at(0) << " " << varType.at(1) << endl;
	subquery = trim(arrWords.at(1));
	
	return VALID;
>>>>>>> master
}

QueryValidator::RETURN_TYPE QueryValidator::findSuchThatString(string &subquery) {
	vector<string> arrWords = split(subquery, SYMBOL_SPACE, 3);
	//cout << "findSuchThatString" << endl;
	if (stringToLower(arrWords.at(0)).compare("such") == 0 &&
		stringToLower(arrWords.at(1)).compare("that") == 0) {
		subquery = arrWords.at(2);
		return VALID;
	}

	return NONE;
}

bool QueryValidator::isValidVariableName(string varName){
	if (varName.length() == 0) {
		return false;
	}

	if (std::find(KEYWORDS.begin(), KEYWORDS.end(), stringToLower(varName)) != KEYWORDS.end()) {
		return false;
	}

	for (int i = 0; i < varName.length(); i++) {
		if (i == 0) {
			if (!isalpha(varName.at(i))) {
				return false;
			}
		}
		else if (!(isalnum(varName.at(i)) || varName.at(i) == '#')) {
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
	line.erase(0, line.find_first_not_of(SYMBOL_SPACE));
	line.erase(line.find_last_not_of(SYMBOL_SPACE) + 1);
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

bool QueryValidator::isVarNameExists(string varName) {
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
	if (str.empty() || !isdigit(str.at(0))) {
		return false;
	}

	char *p;
	strtol(str.c_str(), &p, 10);

	return (*p == 0);
}
