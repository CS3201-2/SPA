#include "QueryTree.h"
using namespace std;

QueryTree::QueryTree() {
	qTree.push_back(variableTree);
	qTree.push_back(suchThatTree);
	qTree.push_back(patternTree);
	qTree.push_back(selectTree);
	qTree.push_back(withTree);
	qTree.push_back(suchThatConstTree);
}

//Inserting the particular type of query into its respective tree
void QueryTree::insertSuchThat(string rel, vector<string> arrVar, vector<string> arrType) {
	vector<string> tempVector;
	vector<string> tempConstVector;
	int i = 0;
	while (i < arrVar.size()) {
		if ((arrType.at(i) == "string" || arrType.at(i) == "number") && (arrType.at(i + 1) == "string" || arrType.at(i + 1) == "number")) {
			tempConstVector.push_back(rel);
			tempConstVector.push_back(arrVar.at(i));
			tempConstVector.push_back(arrType.at(i));
			tempConstVector.push_back(arrVar.at(i+1));
			tempConstVector.push_back(arrType.at(i+1));
		}
		else {
			tempVector.push_back(rel);
			tempVector.push_back(arrVar.at(i));
			tempVector.push_back(arrType.at(i));
			tempVector.push_back(arrVar.at(i+1));
			tempVector.push_back(arrType.at(i+1));
		}
		i = i + 2;
	}
	qTree.at(1).push_back(tempVector);
	qTree.at(5).push_back(tempConstVector);
}

void QueryTree::insertPattern(string syn, string synType, vector<string> arrPtrn, vector<string> ptrnType) {
	vector<string> tempVector;
	tempVector.push_back(syn);
	tempVector.push_back(synType);
	for (int i = 0; i < arrPtrn.size(); i++) {
		tempVector.push_back(arrPtrn.at(i));
		tempVector.push_back(ptrnType.at(i));
	}
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

void QueryTree::insertWith(string rel, vector<string> arrWith, vector<string> withType) {
	vector<string> tempVector;
	tempVector.push_back(rel);
	for (int i = 0; i < arrWith.size(); i++) {
		tempVector.push_back(arrWith.at(i));
		tempVector.push_back(withType.at(i));
	}
	qTree.at(4).push_back(tempVector);
}

//Obtaining the size of the tree of each of the query types
int QueryTree::getSuchThatSize() {
	return qTree.at(1).size();
}

int QueryTree::getSuchThatConstSize() {
	return qTree.at(5).size();
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

int QueryTree::getWithSize() {
	return qTree.at(4).size();
}

//Obtaining the queries for the respective query types
vector<string> QueryTree::getSuchThatQuery(int queryPos) {
	return qTree.at(1).at(queryPos);
}

vector<string> QueryTree::getSuchThatConstQuery(int queryPos) {
	return qTree.at(5).at(queryPos);
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

vector<string> QueryTree::getWithQuery(int queryPos) {
	return qTree.at(4).at(queryPos);
}

