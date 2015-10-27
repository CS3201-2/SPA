#include "Pattern.h"


Pattern::Pattern() {
}

void Pattern::setPattern(int stmtNo, string first, string second) {

	if (second == "_if_") {
		insertToMap(ifMap, first, stmtNo);
	}
	else if (second == "_while_") {
		insertToMap(whileMap, first, stmtNo);
	}
	else {
		string secondPostFix = convertToPostFix(second);
		pair<string, string> p = make_pair(first, secondPostFix);
		assignMap[stmtNo] = p;
		
		insertToMap(assignLeftMap, first, stmtNo);
		list<string> varList = getVariableList(second);
		for (list<string>::iterator it = varList.begin(); it != varList.end(); ++it) {
			insertToMap(assignRightMap, *it, stmtNo);
		}
	}
}

void Pattern::insertToMap(map<string, list<int>>& m, string var, int stmtNo) {
	list<int> temp;
	if (m.find(var) == m.end()) {
		temp.push_back(stmtNo);
		m[var] = temp;
	}
	else {
		temp = m.at(var);
		temp.push_back(stmtNo);
		m[var] = temp;
	}
}

list<int> Pattern::getAssignWithFirstExact(string first) {
	string trimmedFirst = removeAllSpace(first);
	if (assignLeftMap.find(trimmedFirst) == assignLeftMap.end()) {
		return list<int>();
	}
	else {
		return assignLeftMap.at(trimmedFirst);
	}
}

list<int> Pattern::getAssignWithSecondExact(string second) {
	list<int> result, temp;
	string trimmedSecond = removeAllSpace(second);
	string secondPostFix = convertToPostFix(trimmedSecond);

	string firstVar = getFirstVar(trimmedSecond);
	if (assignRightMap.find(firstVar) == assignRightMap.end()) {
		return list<int>();
	}
	else {
		temp = assignRightMap.at(firstVar);
	}

	for (list<int>::iterator it = temp.begin(); it != temp.end(); ++it) {
		if (assignMap.at(*it).second == secondPostFix) {
			result.push_back(*it);
		}
	}

	return result;
}

list<int> Pattern::getAssignWithSecond(string second) {
	list<int> result, temp;	
	string trimmedSecond = removeAllSpace(second);
	string secondPostFix = convertToPostFix(trimmedSecond);

	string firstVar = getFirstVar(trimmedSecond);
	if (assignRightMap.find(firstVar) == assignRightMap.end()) {
		return list<int>();
	}
	else {
		temp = assignRightMap.at(firstVar);
	}

	logPattern();
	string str;
	for (auto&x : temp) {
		str += to_string(x) + ", ";
	}
	SPALog::log(str);

	for (list<int>::iterator it = temp.begin(); it != temp.end(); ++it) {
		if (assignMap.at(*it).second.find(secondPostFix) != string::npos) {
			result.push_back(*it);
		}
	}

	return result;
}

list<int> Pattern::getAssignWithBothExact(string first, string second) {
	list<int> result, temp1, temp2;
	string trimmedSecond = removeAllSpace(second);
	string secondPostFix = convertToPostFix(trimmedSecond);

	temp1 = getAssignWithFirstExact(first);
	temp2 = getAssignWithSecondExact(second);
	set_intersection(temp1.begin(), temp1.end(), temp2.begin(), temp2.end(), back_inserter(result));
	return result;

	/*for (list<int>::iterator it = temp.begin(); it != temp.end(); ++it) {
		if (assignMap.at(*it).second == secondPostFix) {
			result.push_back(*it);
		}
	}

	return result;*/
}

list<int> Pattern::getAssignWithBoth(string first, string second) {
	list<int> result, temp1, temp2;
	string trimmedSecond = removeAllSpace(second);
	string secondPostFix = convertToPostFix(trimmedSecond);

	temp1 = getAssignWithFirstExact(first);
	temp2 = getAssignWithSecond(second);
	set_intersection(temp1.begin(), temp1.end(), temp2.begin(), temp2.end(), back_inserter(result));
	return result;
	/*

	list<string> varList = getFirstVar(trimmedSecond);
	temp = mergeList(varList);

	for (list<int>::iterator it = temp.begin(); it != temp.end(); ++it) {
		if (assignMap.at(*it).second.find(secondPostFix) != string::npos) {
			result.push_back(*it);
		}
	}

	return result;*/
}

list<int> Pattern::getIfWithFirstExact(string first) {
	string trimmedFirst = removeAllSpace(first);
	if (ifMap.find(trimmedFirst) == ifMap.end()) {
		return list<int>();
	}
	else {
		return ifMap.at(trimmedFirst);
	}
}

list<int> Pattern::getWhileWithFisrtExact(string first) {
	string trimmedFirst = removeAllSpace(first);
	if (whileMap.find(trimmedFirst) == whileMap.end()) {
		return list<int>();
	}
	else {
		return whileMap.at(trimmedFirst);
	}
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

//input will not have ;
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
		result.push_back(temp);

		expression = expression.substr(found + 1);
		found = expression.find_first_of("=+-*()");
	}

	if (!expression.empty()) {
		result.push_back(expression);
	}

	return result;
}

list<string> Pattern::getVariableList(string expression) {
	size_t found = expression.find_first_of("(=+-*)");
	list<string> result;
	string temp;

	while (found != string::npos) {
		temp = expression.substr(0, found);
		if (temp != "") {
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


string Pattern::getFirstVar(string expression) {
	size_t found = expression.find_first_of("+-*");

	if (found != string::npos) {
		string temp = expression.substr(0, found);
		temp.erase(std::remove(temp.begin(), temp.end(), '('), temp.end());
		return temp;
	}
	else {
		return expression;
	}
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
	string str =  "pattern assign table\n";

	for (map<int, pair<string, string>>::iterator it = assignMap.begin(); it != assignMap.end(); ++it) {
		str += to_string((*it).first) + ": " + (*it).second.first + "       ||||      " + (*it).second.second + "\n";
	}
	str += "\n";

	str += "pattern if table\n";
	for (map<string, list<int>>::iterator it = ifMap.begin(); it != ifMap.end(); ++it) {
		str += (*it).first + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += to_string(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "pattern while table\n";
	for (map<string, list<int>>::iterator it = whileMap.begin(); it != whileMap.end(); ++it) {
		str += (*it).first + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += to_string(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "pattern assign left table\n";
	for (map<string, list<int>>::iterator it = assignLeftMap.begin(); it != assignLeftMap.end(); ++it) {
		str += (*it).first + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += to_string(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	str += "pattern assign right table\n";
	for (map<string, list<int>>::iterator it = assignRightMap.begin(); it != assignRightMap.end(); ++it) {
		str += (*it).first + ": ";
		for (list<int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			str += to_string(*it2) + ", ";
		}
		str += "\n";
	}
	str += "\n";

	SPALog::log(str);
}

string Pattern::removeAllSpace(string str) {
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

void Pattern::sortAndUnifyMap() {
	for (map<string, list<int>>::iterator it = ifMap.begin(); it != ifMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
	for (map<string, list<int>>::iterator it = assignRightMap.begin(); it != assignRightMap.end(); ++it) {
		(*it).second.sort();
		(*it).second.unique();
	}
}