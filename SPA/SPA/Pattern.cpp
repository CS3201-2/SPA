#include "Pattern.h"

Pattern::Pattern() {
}

void Pattern::setPattern(int stmtLine, string LHSStr, string RHSStr) {
	string LHSPostFix = convertToPostFix(LHSStr);
	string RHSPostFix = convertToPostFix(RHSStr);

	pair<string, string> p = make_pair(LHSPostFix, RHSPostFix);
	patternMap[stmtLine] = p;
}

string Pattern::convertToPostFix(string str) {
	stack<string> opStack;
	list<string> tokenList;
	list<string> postfixList;
	string variable = "";
	string result = "";

	for (string::iterator it = str.begin(); it != str.end(); ++it) {
		string temp(1, *it);
		if (isParenthesis(temp) || isOperator(temp)) {
			if (variable != "") {
				tokenList.push_back(variable);
			}
			variable = "";

			tokenList.push_back(temp);
		}
		else {
			variable += temp;
		}
	}
	if (variable != "") {
		tokenList.push_back(variable);
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

int Pattern::getPriority(string str) {
	if (str == "*" || str == "/") {
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
	return (str == "+" || str == "-" || str == "*" || str == "/");
}

void Pattern::printMap() {
	for (map<int, pair<string, string>>::iterator it = patternMap.begin(); it != patternMap.end(); ++it) {
		cout << (*it).first;
		cout << ":";
		cout << (*it).second.first;
		cout << ", ";
		cout << (*it).second.second;
		cout << endl;
	}
}