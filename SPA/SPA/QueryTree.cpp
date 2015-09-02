#include "QueryTree.h"
using namespace std;

QueryTree::QueryTree() {
}

void QueryTree::insertSuchThat(string rel, string arg1, string arg1Type, string arg2, string arg2Type) {
	vector<string> tempVector;
	tempVector.push_back(rel);
	tempVector.push_back(arg1);
	tempVector.push_back(arg1Type);
	tempVector.push_back(arg2);
	tempVector.push_back(arg2Type);
	suchThatTree.push_back(tempVector);
}