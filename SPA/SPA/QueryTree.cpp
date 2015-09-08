#include "QueryTree.h"
using namespace std;

QueryTree::QueryTree() {
	qTree.push_back(variableTree);
	qTree.push_back(suchThatTree);
	qTree.push_back(patternTree);
	qTree.push_back(selectTree);
}

//Inserting the particular type of query into its respective tree
void QueryTree::insertSuchThat(string rel, vector<string> arrVar, vector<string> arrType) {
	vector<string> tempVector;
	tempVector.push_back(rel);
	tempVector.push_back(arrVar.at(0));
	tempVector.push_back(arrType.at(0));
	tempVector.push_back(arrVar.at(1));
	tempVector.push_back(arrType.at(1));
	qTree.at(1).push_back(tempVector);
}

void QueryTree::insertPattern(string syn, string synType, vector<string> arrPtrn, vector<string> ptrnType) {
	vector<string> tempVector;
	tempVector.push_back(syn);
	tempVector.push_back(synType);
	tempVector.push_back(arrPtrn.at(0));
	tempVector.push_back(ptrnType.at(0));
	tempVector.push_back(arrPtrn.at(1));
	tempVector.push_back(ptrnType.at(1));
	qTree.at(2).push_back(tempVector);
}

void QueryTree::insertVariable(string variable, string variableType) {
	vector<string> tempVector;
	tempVector.push_back(variable);
	tempVector.push_back(variableType);
	qTree.at(0).push_back(tempVector);
}

void QueryTree::insertSelect(string var, string varType) {
	vector<string> tempVector;
	tempVector.push_back(var);
	tempVector.push_back(varType);
	qTree.at(3).push_back(tempVector);
}

//Obtaining the size of the tree of each of the query types
int QueryTree::getSuchThatSize() {
	return qTree.at(1).size();
}

int QueryTree::getPatternSize() {
	return qTree.at(2).size();
}

int QueryTree::getVariableSize() {
	return qTree.at(0).size();
}

int QueryTree::getSelectSize() {
	return qTree.at(3).size();
}

//Obtaining the queries for the respective query types
vector<string> QueryTree::getSuchThatQuery(int queryPos) {
	return qTree.at(1).at(queryPos);
}

vector<string> QueryTree::getPatternQuery(int queryPos) {
	return qTree.at(2).at(queryPos);
}

vector<string> QueryTree::getVariableQuery(int queryPos) {
	return qTree.at(0).at(queryPos);
}

vector<string> QueryTree::getSelectQuery(int queryPos) {
	return qTree.at(3).at(queryPos);
}

