#include "Pattern.h"


Pattern::Pattern() {
}

void Pattern::setPattern(int stmtNo, string first, string second) {
	string firstPostFix = convertToPostFix(first);
	string secondPostFix = convertToPostFix(second);

	pair<string, string> p = make_pair(firstPostFix, secondPostFix);
	patternMap[stmtNo] = p;
}

list<int> Pattern::getAssignWithFirstExact(string first) {
	list<int> result;
	string trimmedFirst = removeAllSpace(first);
	string firstPostFix = convertToPostFix(trimmedFirst);
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		if ((*it).second.first == firstPostFix && 
			((*it).second.second != "_if_" || (*it).second.second != "_while_")) {
			result.push_back((*it).first);
		}
	}
	return result;
}

list<int> Pattern::getAssignWithSecondExact(string second) {
	list<int> result;
	string trimmedSecond = removeAllSpace(second);
	string secondPostFix = convertToPostFix(trimmedSecond);
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		if ((*it).second.second == secondPostFix) {
			result.push_back((*it).first);
		}
	}
	return result;
}

list<int> Pattern::getAssignWithSecond(string second) {
	list<int> result;	
	string trimmedSecond = removeAllSpace(second);
	string secondPostFix = convertToPostFix(trimmedSecond);
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		if (((*it).second.second).find(secondPostFix) != string::npos) {
			result.push_back((*it).first);
		}
	}
	return result;
}

list<int> Pattern::getAssignWithBothExact(string first, string second) {
	list<int> result;
	string trimmedFirst = removeAllSpace(first);
	string trimmedSecond = removeAllSpace(second);
	string firstPostFix = convertToPostFix(trimmedFirst);
	string secondPostFix = convertToPostFix(trimmedSecond);
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		if ((*it).second.first == firstPostFix && (*it).second.second == secondPostFix) {
			result.push_back((*it).first);
		}
	}
	return result;
}

list<int> Pattern::getAssignWithBoth(string first, string second) {
	list<int> result;
	string trimmedFirst = removeAllSpace(first);
	string trimmedSecond = removeAllSpace(second);
	string firstPostFix = convertToPostFix(trimmedFirst);
	string secondPostFix = convertToPostFix(trimmedSecond);
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		if ((*it).second.first == firstPostFix && ((*it).second.second).find(secondPostFix) != string::npos) {
			result.push_back((*it).first);
		}
	}
	return result;
}

list<int> Pattern::getIfWithFirstExact(string first) {
	list<int> result;
	string trimmedFirst = removeAllSpace(first);
	string firstPostFix = convertToPostFix(trimmedFirst);
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		if ((*it).second.first == firstPostFix && (*it).second.second == "._if_.") {
			result.push_back((*it).first);
		}
	}
	return result;
}

list<int> Pattern::getWhileWithFisrtExact(string first) {
	list<int> result;
	string trimmedFirst = removeAllSpace(first);
	string firstPostFix = convertToPostFix(trimmedFirst);
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		if ((*it).second.first == firstPostFix && (*it).second.second == "._while_.") {
			result.push_back((*it).first);
		}
	}
	return result;
}

string Pattern::convertToPostFix(string str) {
	stack<string> opStack;
	list<string> tokenList;
	list<string> postfixList;
	string variable = "";
	string result = "";

	tokenList = parseExpression(str);

	if (tokenList.size() == 1) {
		result = "." + tokenList.front() + ".";
		return result;
	}

	for (list<string>::iterator iter = tokenList.begin(); iter != tokenList.end(); ++iter) {
		if (*iter == "(") {
			opStack.push("(");
		}
		else if (*iter == ")") {
			while (opStack.top() != "(") {
				postfixList.push_back(opStack.top());
				opStack.pop();
			}
			opStack.pop();
		}
		else if (isOperator(*iter)) {
			while (!opStack.empty() &&
				(getPriority(opStack.top()) >= getPriority(*iter))) {
				postfixList.push_back(opStack.top());
				opStack.pop();
			}
			opStack.push(*iter);
		}
		else {
			string str = "." + *iter + ".";
			postfixList.push_back(str);
		}
	}

	while (!opStack.empty()) {
		postfixList.push_back(opStack.top());
		opStack.pop();
	}

	for (list<string>::iterator i = postfixList.begin(); i != postfixList.end(); ++i) {
		result += *i;
	}

	return result;
}

list<string> Pattern::parseExpression(string expression) {
	size_t found = expression.find_first_of("(=+-*)");
	list<string> result;
	string temp;

	if (found == string::npos) {
		result.push_back(expression);
		return result;
	}

	while (found != string::npos) {
		temp = expression.substr(0, found);
		if (temp != "") {
			result.push_back(temp);
		}

		temp = expression.at(found);
		if (temp != ";") {
			result.push_back(temp);
		}

		expression = expression.substr(found + 1);
		found = expression.find_first_of("=+-*()");
	}

	if (!expression.empty()) {
		result.push_back(expression);
	}

	return result;
}

int Pattern::getPriority(string str) {
	if (str == "*") {
		return 2;
	}
	else if (str == "+" || str == "-") {
		return 1;
	}
	else {
		return 0;
	}
}

bool Pattern::isParenthesis(string str) {
	return str == "(" || str == ")";
}

bool Pattern::isOperator(string str) {
	return (str == "+" || str == "-" || str == "*");
}

void Pattern::logPattern() {
	string str =  "pattern table\n";

	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		str += to_string((*it).first) + ": " + (*it).second.first + "       ||||      " + (*it).second.second + "\n";
	}
	str += "\n";

	SPALog::log(str);
}

string Pattern::removeAllSpace(string str) {
	string result;
	for (string::iterator it = str.begin(); it != str.end(); ++it) {
		if (*it != ' ') {
			result += *it;
		}
	}

	return result;
}

