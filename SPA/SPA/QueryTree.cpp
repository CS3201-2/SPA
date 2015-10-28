#include "QueryTree.h"
using namespace std;

const string NO_RELATIONSHIP = "";


QueryTree::QueryTree() {
	qTree.push_back(variableTree);
	qTree.push_back(suchThatTree);
	qTree.push_back(patternTree);
	qTree.push_back(selectTree);
	qTree.push_back(withTree);
	qTree.push_back(suchThatConstTree);
	qTree.push_back(withConstTree);
}

//Inserting the particular type of query into its respective tree
void QueryTree::insertSuchThat(string rel, vector<string> arrVar, vector<string> arrType) {
	if ((arrType.at(0) == "string" || arrType.at(0) == "number") && (arrType.at(1) == "string" || arrType.at(1) == "number")) {
		Clause clause = Clause(rel, arrVar, arrType);
		qTree.at(5).push_back(clause);
	}
	else {
		Clause clause = Clause(rel, arrVar, arrType);
		qTree.at(1).push_back(clause);
	}
}

void QueryTree::insertPattern(string syn, string synType, vector<string> arrPtrn, vector<string> ptrnType) {
	arrPtrn.push_back(syn);
	ptrnType.push_back(synType);
	Clause clause = Clause(NO_RELATIONSHIP, arrPtrn, ptrnType);
	qTree.at(2).push_back(clause);
}

void QueryTree::insertVariable(string variable, string variableType) {
	vector<string> varVector;
	varVector.push_back(variable);
	vector<string> varTypeVector;
	varTypeVector.push_back(variableType);
	Clause clause = Clause(NO_RELATIONSHIP, varVector, varTypeVector);
	qTree.at(0).push_back(clause);
}

void QueryTree::insertSelect(vector<string> var, vector<string> varType) {
	Clause clause = Clause(NO_RELATIONSHIP, var, varType);
	qTree.at(3).push_back(clause);
}

void QueryTree::insertSelect(string var, string varType) {
	vector<string> varVector;
	varVector.push_back(var);
	vector<string> varTypeVector;
	varTypeVector.push_back(varType);
	Clause clause = Clause(NO_RELATIONSHIP, varVector, varTypeVector);
	qTree.at(3).push_back(clause);
}

void QueryTree::insertWith(string rel, vector<string> arrWith, vector<string> withType) {
	vector<string> tempVector;
	vector<string> tempConstVector;
	if ((withType.at(0) == "string" || withType.at(0) == "number") && (withType.at(1) == "string" || withType.at(1) == "number")) {
		Clause clause = Clause(rel, arrWith, withType);
		qTree.at(6).push_back(clause);
	}
	else {
		Clause clause = Clause(rel, arrWith, withType);
		qTree.at(4).push_back(clause);
	}
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

int QueryTree::getWithConstSize() {
	return qTree.at(6).size();
}

//Obtaining the queries for the respective query types
Clause QueryTree::getSuchThatQuery(int queryPos) {
	return qTree.at(1).at(queryPos);
}

Clause QueryTree::getSuchThatConstQuery(int queryPos) {
	return qTree.at(5).at(queryPos);
}

Clause QueryTree::getPatternQuery(int queryPos) {
	return qTree.at(2).at(queryPos);
}

Clause QueryTree::getVariableQuery(int queryPos) {
	return qTree.at(0).at(queryPos);
}

Clause QueryTree::getSelectQuery(int queryPos) {
	return qTree.at(3).at(queryPos);
}

Clause QueryTree::getWithQuery(int queryPos) {
	return qTree.at(4).at(queryPos);
}

Clause QueryTree::getWithConstQuery(int queryPos) {
	return qTree.at(6).at(queryPos);
}
