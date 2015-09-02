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
	vector<vector<string>> selectTree;

public:
	QueryTree();

	void insertSuchThat(string rel, string arg1, string arg1_type, string arg2, string arg2_type);
	
};