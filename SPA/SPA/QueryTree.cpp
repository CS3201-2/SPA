#include "QueryTree.h"
using namespace std;

QueryTree::QueryTree() {
}

//Inserting the particular type of query into its respective tree
void QueryTree::insertSuchThat(string rel, string arg1, string arg1Type, string arg2, string arg2Type) {
	vector<string> tempVector;
	tempVector.push_back(rel);
	tempVector.push_back(arg1);
	tempVector.push_back(arg1Type);
	tempVector.push_back(arg2);
	tempVector.push_back(arg2Type);
	suchThatTree.push_back(tempVector);
}

void QueryTree::insertPattern(string syn, string synType, string ptrn1, string ptrn1Type, string ptrn2, string ptrn2Type) {
	vector<string> tempVector;
	tempVector.push_back(syn);
	tempVector.push_back(synType);
	tempVector.push_back(ptrn1);
	tempVector.push_back(ptrn1Type);
	tempVector.push_back(ptrn2);
	tempVector.push_back(ptrn2Type);
	patternTree.push_back(tempVector);
}

void QueryTree::insertVariable(string variable, string variableType) {
	vector<string> tempVector;
	tempVector.push_back(variable);
	tempVector.push_back(variableType);
	variableTree.push_back(tempVector);
}

//Obtaining the size of the tree of each of the query types
int QueryTree::getSuchThatSize() {
	return suchThatTree.size();
}

int QueryTree::getPatternSize() {
	return patternTree.size();
}

int QueryTree::getVariableSize() {
	return variableTree.size();
}


//Obtaining the queries for the respective query types
vector<string> QueryTree::getSuchThatQuery(int queryPos) {
	return suchThatTree[queryPos];
}

vector<string> QueryTree::getPatternQuery(int queryPos) {
	return patternTree[queryPos];
}

vector<string> QueryTree::getVariableQuery(int queryPos) {
	return variableTree[queryPos];
}

