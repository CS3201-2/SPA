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

	void insertSuchThat(string rel, string val1, string val1_type, string val2, string val2_type);
	void insertPattern(string synonym, string synonymType, string pattern1, string pattern1Type, 
						string pattern2, string pattern2Type);
	
};