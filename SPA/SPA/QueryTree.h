#include <string>
#include <vector>
#include <iostream>

using namespace std;
#pragma once

class QueryTree {
private:
	vector<vector<string>> suchThatTree;
	vector<vector<string>> patternTree;
	vector<vector<string>> withTree;
	vector<vector<string>> variableTree;

public:
	QueryTree();

	void insertSuchThat(string rel, string arg1, string arg1_type, string arg2, string arg2_type);
	void insertPattern(string syn, string synType, string ptrn1, string ptrn1Type, string ptrn2, string ptrn2Type);
	void insertVariable(string variable, string variableType);

	int getSuchThatSize();
	int getPatternSize();
	int getVariableSize();
};