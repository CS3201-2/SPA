#include "QueryValidator.h"
#include "SPALog.h"
#include <iostream>  //remove when submitting
#include <regex>

const char SYMBOL_SEMICOLON = ';';
const char SYMBOL_SPACE = ' ';
const char SYMBOL_COMMA = ',';
//const char SYMBOL_UNDERSCORE = '_';
const char SYMBOL_QUOTE = '\"';
const char SYMBOL_OPEN_BRACKET = '(';
const char SYMBOL_CLOSE_BRACKET = ')';
const char SYMBOL_EQUALS = '=';
const char SYMBOL_FULL_STOP = '.';

const vector<string> KEYWORDS = { "constant", "stmt", "stmtLst", "assign", "while", 
	"if", "procedure", "call", "prog_line", "Select", "Modifies", "Uses", "Parent",
	"Parent*", "Follows", "Follows*", "Pattern", "Next", "Next*", "Calls", "Calls*", 
	"Affects", "Affects*", "with", "and", "such", "that" };

const vector<string> VARTYPES = { "variable", "constant", "stmt", 
	"stmtLst", "assign", "while", "if", "procedure", "call", "prog_line" };

const string VARTYPE_VARIABLE = "variable";
const string VARTYPE_STRING = "string";
const string VARTYPE_SUBSTRING = "substring";
const string VARTYPE_CONSTANT = "constant";
const string VARTYPE_ASSIGN = "assign";
const string VARTYPE_WHILE = "while";
const string VARTYPE_IF = "if";
const string VARTYPE_PROCNAME = "procName";
const string VARTYPE_VARNAME = "varName";
const string VARTYPE_PROG_LINE = "prog_line";
const string VARTYPE_NUMBER = "number";
const string VARTYPE_ALL = "all";
const string VARTYPE_BOOLEAN = "BOOLEAN";

const string RELTYPE_SELECT = "Select";

const string RELTYPE_SUCH_THAT = "such that";

const string RELTYPE_PATTERN = "pattern";
const string RELTYPE_PATTERN_ASSIGN = "patternAssign";
const string RELTYPE_PATTERN_WHILE = "patternWhile";
const string RELTYPE_PATTERN_IF = "patternIf";

const string RELTYPE_WITH = "with";
const string RELTYPE_WITH_NUMBER = "withNumber";
const string RELTYPE_WITH_NAME = "withName";

const string RELTYPE_AND = "and";

const regex variableNameRegex("(^[[:alpha:]])([[:alnum:]]+)*$");

bool QueryValidator::isValidDecAndQuery(string query) {
	string q = trim(query);
	vector<string> splitStr = split(q, SYMBOL_SEMICOLON);
	int size = splitStr.size(), i;

	for (i = 0; i < size - 1; i++) {
		if (!isValidDeclaration(splitStr.at(i))) {
			cout << "Invalid Query Declaration" << endl;
			SPALog::log("Invalid Query Declaration: " + q);
			return false;
		}
	}

	if (!isValidQuery(splitStr.at(i))) {
		cout << "Invalid Query" << endl;
		SPALog::log("Invalid Query: " + q);
		return false;
	}

	cout << "Valid Query";
	SPALog::log("Valid Query: " + q);
	return true;
}

QueryTree QueryValidator::getQueryTree() {
	return qt;
}

bool QueryValidator::isValidDeclaration(string declaration) {
	//cout << "isValidDeclaration" << endl;
	vector<string> arrDec = split(declaration, SYMBOL_SPACE, 2);

	if (arrDec.size() < 2 || find(VARTYPES.begin(), VARTYPES.end(), arrDec.at(0)) == VARTYPES.end()) {
		return false;
	}

	vector<string> synonyms = split(arrDec.at(1), SYMBOL_COMMA);

	if (synonyms.size() < 1) {
		return false;
	}
	
	for (int i = 0; i < synonyms.size(); i++) {
		if (!isValidVariableName(synonyms.at(i)) || isVarNameExists(synonyms.at(i))) {
			return false;
		} else {
			varMap[synonyms.at(i)] = arrDec.at(0);
			qt.insertVariable(synonyms.at(i), arrDec.at(0));
		}
	}
	
	return true;
}

bool QueryValidator::isValidQuery(string query) {
	//cout << "isValidQuery\n";
	vector<string> arrClauses = split(query, SYMBOL_SPACE, 2); //don't split into 3 as will have tuples(multiple var) later

	if (arrClauses.at(0).compare(RELTYPE_SELECT) != 0 || arrClauses.size() != 2) {
		return false;
	}

	if (arrClauses.at(1).at(0) == '<') {
		arrClauses = split(arrClauses.at(1), '>', 2);

		arrClauses.at(0) = arrClauses.at(0).substr(1, arrClauses.at(0).size() - 1);

		vector<string> arrWords = split(arrClauses.at(0), SYMBOL_COMMA);
		vector<string> var(arrWords.size()), varType(arrWords.size());

		for (int i = 0; i < arrWords.size(); i++) {
			if (!isVarNameExists(arrWords.at(i))) {
				return false;
			} 

			var.at(i) = arrWords.at(i);
			varType.at(i) = getVarType(arrWords.at(i));
		}
		
		cout << "Select: ";
		for (int i = 0; i < var.size(); i++) {
			cout << i<<" "<<var.at(i) <<" ";
		}

		cout << endl;

		for (int i = 0; i < var.size(); i++) {
			cout << i << " " <<varType.at(i) << " ";
		}

		cout << endl;
		qt.insertSelect(var, varType);
	} else {
		arrClauses = split(arrClauses.at(1), SYMBOL_SPACE, 2);

		if ((!isVarNameExists(arrClauses.at(0)) && arrClauses.at(0).compare(VARTYPE_BOOLEAN) != 0)){
			return false;
		}

		if (isVarNameExists(arrClauses.at(0))) {
			qt.insertSelect(arrClauses.at(0), getVarType(arrClauses.at(0)));
		} else {
			qt.insertSelect("", stringToLower(VARTYPE_BOOLEAN));
		}
	}

	if (arrClauses.size() == 1) {
		return true;
	}

	string prevClause = "";
	bool retVal;
	
	while (1) {
		if ((arrClauses.at(1).size() > 0 && arrClauses.at(1).at(0) == NULL) || arrClauses.at(1).size() == 0) {
			break;
		}
		
		arrClauses = split(arrClauses.at(1), SYMBOL_SPACE, 2);

		if (arrClauses.size() != 2) {
			return false;
		}
		
		if (arrClauses.at(0).compare("such") == 0) {
			arrClauses = split(arrClauses.at(1), SYMBOL_SPACE, 2);

			if (arrClauses.size() != 2 || arrClauses.at(0).compare("that") != 0) {
				return false;
			}

			retVal = parseRel(arrClauses.at(1));
			prevClause = RELTYPE_SUCH_THAT;

		} else if (arrClauses.at(0).compare(RELTYPE_PATTERN) == 0) {
			retVal = findPatternClause(arrClauses.at(1));
			prevClause = RELTYPE_PATTERN;

		} else if (arrClauses.at(0).compare(RELTYPE_WITH) == 0) {
			retVal = findWithClause(arrClauses.at(1));
			prevClause = RELTYPE_WITH;

		} else if (arrClauses.at(0).compare(RELTYPE_AND) == 0) {
			
			if (prevClause.compare(RELTYPE_SUCH_THAT) == 0) {
				retVal = parseRel(arrClauses.at(1));
				prevClause = RELTYPE_SUCH_THAT;

			} else if (prevClause.compare(RELTYPE_PATTERN) == 0) {
				retVal = findPatternClause(arrClauses.at(1));
				prevClause = RELTYPE_PATTERN;

			} else if (prevClause.compare(RELTYPE_WITH) == 0) {
				retVal = findWithClause(arrClauses.at(1));
				prevClause = RELTYPE_WITH;

			} else {
				return false;
			}
		} else {
			break;
		}
	
		if (!retVal) {
			return false;
		} 
	}
	
	if (arrClauses.at(1).size() > 0 && arrClauses.at(1).at(0) != NULL) {
		return false;
	}

	return true;
}

bool QueryValidator::parseRel(string &subquery) {
	//cout << "parseRel" << endl;
	vector<string> arrClauses = split(subquery, SYMBOL_OPEN_BRACKET, 2);
	
	if (!r.hasRelationship(stringToLower(arrClauses.at(0))) || arrClauses.size() != 2) {
		return false;
	}
	
	string relType = stringToLower(arrClauses.at(0));
	
	if (arrClauses.at(1).find(")") == string::npos) {
		return false;
	}
	
	arrClauses = split(arrClauses.at(1), SYMBOL_CLOSE_BRACKET, 2);

	if (arrClauses.size() != 2) {
		return false;
	}

	vector<string> arrVar = split(arrClauses.at(0), SYMBOL_COMMA);

	if (!r.isNumOfArgsEqual(relType, arrVar.size())) {
		return false;
	}

	vector<string> varTypes(arrVar.size());

	if (!parseRelArgs(relType, arrVar, varTypes)) {
		return false;
	}

	qt.insertSuchThat(relType, arrVar, varTypes);
	
	cout << "such that: " << relType << " " << arrVar.at(0) << " " << arrVar.at(1) << endl;
	//cout << varTypes.at(0) << " " << varTypes.at(1) << endl;
	subquery = trim(arrClauses.at(1));
	return true;
}

bool QueryValidator::parseRelArgs(string relType,
	vector<string>& arrVar, vector<string>& varTypes) {
	//cout << "parseRelArgs";
	for (int i = 0; i < arrVar.size(); i++) {
		if (isVarNameExists(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, getVarType(arrVar.at(i)))) {
				return false;
			} else {
				varTypes.at(i) = getVarType(arrVar.at(i));
			}

		} else if (isStringVar(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, VARTYPE_STRING)) {
				return false;
			} else {
				arrVar.at(i) = arrVar.at(i).substr(1, arrVar.at(i).size() - 2);
				varTypes.at(i) = VARTYPE_STRING;
			}

		} else if (isPositiveInteger(arrVar.at(i))) {
			if (!r.isArgValid(relType, i + 1, VARTYPE_NUMBER)) {
				return false;
			} else {
				varTypes.at(i) = VARTYPE_NUMBER;
			}

		} else if (arrVar.at(i).compare("_") == 0) {
			if (!r.isArgValid(relType, i + 1, VARTYPE_ALL)) {
				return false;
			} else {
				varTypes.at(i) = VARTYPE_ALL;
			}

		} else {
			return false;
		}
	}
	
	return true;
}

bool QueryValidator::findPatternClause(string &subquery) {
	vector<string> arrWords = split(subquery, SYMBOL_OPEN_BRACKET, 2);

	if (arrWords.size() != 2) {
		return false;
	}
	
	if (isVarNameExists(arrWords.at(0))) {
		if (getVarType(arrWords.at(0)).compare(VARTYPE_ASSIGN) == 0 ||
			getVarType(arrWords.at(0)).compare(VARTYPE_WHILE) == 0 ||
			getVarType(arrWords.at(0)).compare(VARTYPE_IF) == 0) {

			string relType, syn, synType;

			if (!parsePatternType(arrWords.at(0), relType, syn, synType)) {
				return false;
			}

			//cout << "parsed patterns\n";
			if (arrWords.at(1).find(")") == string::npos) {
				return false;
			}
			
			int openBrac = arrWords.at(1).find("("), closeBrac = arrWords.at(1).find(")");

			if (closeBrac != string::npos && (openBrac > closeBrac || openBrac == string::npos)) {
				arrWords = split(arrWords.at(1), SYMBOL_CLOSE_BRACKET, 2);
			} else if (closeBrac != string::npos && openBrac < closeBrac) {
				int countClose = 1, countOpen = 1;
				
				while (countClose <= countOpen && (openBrac != string::npos 
						|| closeBrac != string::npos)) {
					
					openBrac = arrWords.at(1).find("(", openBrac + 1);
					closeBrac = arrWords.at(1).find(")", closeBrac + 1);
					
					if (openBrac != string::npos) {
						countOpen++;
					}

					if (closeBrac != string::npos) {
						countClose++;
					}
				}
				
				string word = arrWords.at(1);
				arrWords.at(0) = trim(word.substr(0, closeBrac));
				arrWords.at(1) = trim(word.substr(closeBrac + 1, word.size() - closeBrac - 1));
			} else {
				return false;
			}

			if (arrWords.size() != 2) {
				return false;
			}
		
			vector<string> arrVar = split(arrWords.at(0), SYMBOL_COMMA);
		
			if (!r.isNumOfArgsEqual(relType, arrVar.size())) {
				return false;
			}
			
			vector<string> varType(arrVar.size());
			//arg1 and 2
			//cout << "arg1 and 2\n";
			if (!parsePatternArg1(relType, arrVar.at(0), varType.at(0))) {
				//cout << "invalid 1st arg\n";
				return false;
			}
			
			//arg2
			if (!parsePatternArg2(relType, arrVar.at(1), varType.at(1))) {
				//cout << "invalid 2nd arg\n";
				return false;
			}
			//cout << "parsed 2nd arg\n";
			if (relType.compare(RELTYPE_PATTERN_IF) == 0 && arrVar.at(2).compare("_") != 0) {
				return false;
			}

			qt.insertPattern(syn, synType, arrVar, varType);

			cout << "pattern: " << arrVar.at(0) << " " << arrVar.at(1) << endl;
			//cout << "pattern type " << varType.at(0) << " " << varType.at(1) << endl;
		}
	} else {
		return false;
	}

	subquery = trim(arrWords.at(1));
	return true;
}

bool QueryValidator::parsePatternType(string word, string &relType, string &syn, 
		string &synType) {
	
	if (getVarType(word).compare(VARTYPE_ASSIGN) == 0) {
		relType = RELTYPE_PATTERN_ASSIGN;
		syn = word;
		synType = VARTYPE_ASSIGN;

	} else if (getVarType(word).compare(VARTYPE_WHILE) == 0) {
		relType = RELTYPE_PATTERN_WHILE;
		syn = word;
		synType = VARTYPE_WHILE;

	} else if (getVarType(word).compare(VARTYPE_IF) == 0) {
		relType = RELTYPE_PATTERN_IF;
		syn = word;
		synType = VARTYPE_IF;

	} else {
		return false;
	}

 return true;
}

bool QueryValidator::parsePatternArg1(string relType, string &arg, string &varType) {

	if (isVarNameExists(arg) && getVarType(arg).compare(VARTYPE_VARIABLE) == 0) {
			varType = VARTYPE_VARIABLE;

	} else if (isStringVar(arg)) {
		arg = arg.substr(1, arg.size() - 2);
		varType = VARTYPE_STRING;

	} else if (arg.compare("_") == 0) {
		varType = VARTYPE_ALL;

	} else {
		return false;
	}

	return true;
}

bool QueryValidator::parsePatternArg2(string relType, string &arg, string &varType) {
	//cout << "parsePatternArg2 "<<arg<<"\n";
	if (arg.compare("_") == 0) {
		varType = VARTYPE_ALL;

	} else if (arg.at(0) == '_' && arg.at(arg.size() - 1) == '_'  &&
		arg.at(1) == SYMBOL_QUOTE && arg.at(arg.size() - 2) == SYMBOL_QUOTE) {

		arg = arg.substr(2, arg.size() - 4);
		
		if (isValidExpression(arg)) {
			cout << "exp = " << arg << endl;
			if (!r.isArgValid(relType, 2, VARTYPE_SUBSTRING)) {
				return false;
			} else {
				
				varType = VARTYPE_SUBSTRING;
			}
		}
	} else if (arg.at(0) == SYMBOL_QUOTE && arg.at(arg.size() - 1) == SYMBOL_QUOTE) {
		arg = arg.substr(1, arg.size() - 2);

		if (isValidExpression(arg)) {
			if (!r.isArgValid(relType, 2, VARTYPE_STRING)) {
				return false;
			} else {
				varType = VARTYPE_STRING;
			}
		}
	} else {
		return false;
	}
	
	return true;
}

bool QueryValidator::findWithClause(string &subquery) {
	string relType;
	vector<string> arrVar(2), varTypes(2);

	if (!parseWithNumber(subquery, relType, arrVar, varTypes)) {
		if (!parseWithName(subquery, relType, arrVar, varTypes)) {
			return false;
		}
	}
	
	cout << "with: " << arrVar.at(0) << " " << arrVar.at(1) << endl;
	//cout << varTypes.at(0) << " " << varTypes.at(1) << endl;

	qt.insertWith(relType, arrVar, varTypes);
	return true;
}

bool QueryValidator::parseWithNumber(string &subquery, string &relType, 
		vector<string> &arrVar, vector<string> &varTypes) {
	//cout << "parseWithNumber\n";
	vector<string> arrWords = split(subquery, SYMBOL_EQUALS, 2);
	relType = RELTYPE_WITH_NUMBER;

	if (!r.isNumOfArgsEqual(relType, arrWords.size())) {
		return false;
	}

	vector<string> query = split(arrWords.at(1), SYMBOL_SPACE, 2);
	arrWords.at(1) = query.at(0);

	for (int i = 0; i < arrWords.size(); i++) {
		
		if ( isPositiveInteger(arrWords.at(i)) ) {
			if (r.isArgValid(relType, i + 1, VARTYPE_NUMBER)) {
				arrVar.at(i) = arrWords.at(i);
				varTypes.at(i) = VARTYPE_NUMBER;
			} else {
				return false;
			}

		} else if (isVarNameExists(arrWords.at(i)) && getVarType(arrWords.at(i)).compare(VARTYPE_PROG_LINE) == 0) {
			if (r.isArgValid(relType, i + 1, VARTYPE_PROG_LINE)) {
				arrVar.at(i) = arrWords.at(i);
				varTypes.at(i) = VARTYPE_PROG_LINE;
			} else {
				return false;
			}

		} else {
			vector<string> variable = split(arrWords.at(i), SYMBOL_FULL_STOP);

			if (isVarNameExists(variable.at(0))) {
				string variabType = getVarType(variable.at(0));

				if (r.isArgValid(relType, i + 1, variabType)) {
					if (variabType.compare(VARTYPE_CONSTANT) == 0) {
						if (variable.at(1).compare("value") == 0) {
							arrVar.at(i) = variable.at(0);
							varTypes.at(i) = variabType;
						} else {
							return false;
						}

					} else {
						if (variable.at(1).compare("stmt#") == 0) {
							arrVar.at(i) = variable.at(0);
							varTypes.at(i) = variabType;
						} else {
							return false;
						}
					}	

				} else {
					return false;
				}
			} else {
				return false;
			}
		}
	}

	if (query.size() == 2) {
		subquery = query.at(1);	
	} else {
		subquery = "";
	}

	return true;
}

bool QueryValidator::parseWithName(string &subquery, string &relType, 
		vector<string> &arrVar, vector<string> &varTypes) {
	//cout << "parseWithName\n";
	vector<string> arrWords = split(subquery, SYMBOL_EQUALS, 2);
	relType = RELTYPE_WITH_NAME;

	if (!r.isNumOfArgsEqual(relType, arrWords.size())) {
		return false;
	}

	vector<string> query = split(arrWords.at(1), SYMBOL_SPACE, 2);

	arrWords.at(1) = query.at(0);

	for (int i = 0; i < arrWords.size(); i++) {
		
		if (isStringVar(arrWords.at(i))) {
			if (r.isArgValid(relType, i + 1, VARTYPE_STRING)) {
				arrVar.at(i) = arrWords.at(i).substr(1, arrWords.at(i).size() - 2);
				varTypes.at(i) = VARTYPE_STRING;
			} else {
				return false;
			}

		} else {
			vector<string> variable = split(arrWords.at(i), SYMBOL_FULL_STOP);
			
			if (isVarNameExists(variable.at(0))) {
				string variabType = getVarType(variable.at(0));
				
				if (r.isArgValid(relType, i + 1, variabType)) {
					if (variabType.compare(VARTYPE_VARIABLE) == 0) {
						if (variable.at(1).compare(VARTYPE_VARNAME) == 0) {
							arrVar.at(i) = variable.at(0);
							varTypes.at(i) = variabType;
						} else {
							return false;
						}

					} else {
						if (variable.at(1).compare(VARTYPE_PROCNAME) == 0) {
							arrVar.at(i) = variable.at(0);
							varTypes.at(i) = variabType;
						} else {
							return false;
						}
					}

				} else {
					return false;
				}
			} else {
				return false;
			}
		}
	}

	if (query.size() == 2) {
		subquery = query.at(1);

	} else {
		subquery = "";
	}

	return true;
}

bool QueryValidator::isValidVariableName(string varName) {
	if (varName.length() == 0) {
		return false;
	}

	if (find(KEYWORDS.begin(), KEYWORDS.end(), varName) != KEYWORDS.end()) {
		return false;
	}

	return regex_match(varName, variableNameRegex);
}

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

string QueryValidator::removeSpaces(string line) {
	line.erase(0, line.find_first_not_of(SYMBOL_SPACE));
	line.erase(line.find_last_not_of(SYMBOL_SPACE) + 1);
	return line;
}

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
	if (!(str.front() == SYMBOL_QUOTE && str.back() == SYMBOL_QUOTE)) {
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
	if (str.empty() || !isdigit(str.at(0)) || str.at(0) == '0') {
		return false;
	}

	char *p;
	strtol(str.c_str(), &p, 10);

	return (*p == 0);
}

vector<string> QueryValidator::parseExpression(string expression) {
	size_t found = expression.find_first_of("(+-*)");
	vector<string> result;
	string temp;

	while (found != string::npos) {
		
		temp = expression.substr(0, found);
		
		if (temp != "" && temp != " ") {
			//cout << "temp = " << temp << endl;
			result.push_back(removeSpaces(temp));
		}

		temp = expression.at(found);
		if (temp != "" && temp != " ") {
			//cout << "temp = " << temp << endl;
			result.push_back(removeSpaces(temp));
		}
		expression = expression.substr(found + 1);
		found = expression.find_first_of("+-*()");
	}

	if (!expression.empty() && expression != " ") {
		//cout << "expression = " << expression << endl;
		result.push_back(removeSpaces(expression));
	}

	/*for (int i = 0; i < result.size(); i++) {
		cout << result.at(i) << endl;
	}*/

	return result;
}

bool QueryValidator::isValidExpression(string expression) {
	vector<string> tokens = parseExpression(expression);

	if (isOperator(tokens.at(0))) {
		return false;
	}

	string prevToken = "dummy";
	for (int i = 0; i < tokens.size(); i++) {
		//cout << "token = " << tokens.at(i) << endl;
		if (!isOperator(tokens.at(i)) && !isValidVariableName(tokens.at(i)) &&
			!isParenthesis(tokens.at(i)) && !isPositiveInteger(tokens.at(i)) ) {
			//cout << "invalid = "<<tokens.at(i)<<endl;
			return false;
		}

		if (isOperator(prevToken) && isOperator(tokens.at(i))) {
			//cout << "isoperator\n";
			return false;
		}
		prevToken = tokens.at(i);
	}

	if (countNumOfLeftParenthesis(expression) != countNumOfRightParenthesis(expression)) {
		//cout << "parenthesis\n";
		return false;
	}

	return true;
}

bool QueryValidator::isOperator(string str) {
	return str == "+" || str == "-" || str == "*";
}

bool QueryValidator::isParenthesis(string str) {
	return str == "(" || str == ")";
}

int QueryValidator::countNumOfLeftParenthesis(string stmt) {
	return count(stmt.begin(), stmt.end(), SYMBOL_OPEN_BRACKET);
}

int QueryValidator::countNumOfRightParenthesis(string stmt) {
	return count(stmt.begin(), stmt.end(), SYMBOL_CLOSE_BRACKET);
}