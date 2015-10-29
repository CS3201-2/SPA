#include "QueryTree.h"
using namespace std;

const string NO_RELATIONSHIP = "";
const int NO_VARIABLE = 0;
const int ONE_VARIABLE = 1;
const int TWO_VARIABLE = 2;
const int FIRST_TYPE = 0;
const string VARIABLE_TYPE = "variable";

QueryTree::QueryTree() {

}

//Inserting the particular type of query into its respective tree
void QueryTree::insertSuchThat(string rel, vector<string> arrVar, vector<string> arrType) {
	int numOfVar = getNumOfVar(arrType);
	Clause clause = Clause(rel, arrVar, arrType);

	if (numOfVar == NO_VARIABLE) {
		suchThatNoVarTree.push_back(clause);
	}
	else if (numOfVar == ONE_VARIABLE) {
		suchThatOneVarTree.push_back(clause);
	}
	else {
		suchThatTwoVarTree.push_back(clause);
	}
}

void QueryTree::insertPattern(string syn, string synType, vector<string> arrPtrn, vector<string> ptrnType) {
	arrPtrn.push_back(syn);
	ptrnType.push_back(synType);
	Clause clause = Clause(NO_RELATIONSHIP, arrPtrn, ptrnType);
	if (ptrnType.at(FIRST_TYPE) == VARIABLE_TYPE) {
		patternOneVarTree.push_back(clause);
	}
	else {
		patternTwoVarTree.push_back(clause);
	}
}

void QueryTree::insertVariable(string variable, string variableType) {
	vector<string> varVector;
	varVector.push_back(variable);
	vector<string> varTypeVector;
	varTypeVector.push_back(variableType);
	Clause clause = Clause(NO_RELATIONSHIP, varVector, varTypeVector);
	variableTree.push_back(clause);
}

void QueryTree::insertSelect(vector<string> var, vector<string> varType) {
	Clause clause = Clause(NO_RELATIONSHIP, var, varType);
	selectTree.push_back(clause);
}

void QueryTree::insertSelect(string var, string varType) {
	vector<string> varVector;
	varVector.push_back(var);
	vector<string> varTypeVector;
	varTypeVector.push_back(varType);
	Clause clause = Clause(NO_RELATIONSHIP, varVector, varTypeVector);
	selectTree.push_back(clause);
}

void QueryTree::insertWith(string rel, vector<string> arrWith, vector<string> withType) {
	int numOfVar = getNumOfVar(withType);
	Clause clause = Clause(rel, arrWith, withType);

	if (numOfVar == NO_VARIABLE) {
		withNoVarTree.push_back(clause);
	}
	else if (numOfVar = ONE_VARIABLE) {
		withOneVarTree.push_back(clause);
	}
	else {
		withTwoVarTree.push_back(clause);
	}
}

vector<Clause> QueryTree::getVariableTree() {
	return variableTree;
}

vector<Clause> QueryTree::getSelectTree() {
	return selectTree;
}

vector<Clause> QueryTree::getSuchThatNoVarTree() {
	return suchThatNoVarTree;
}

vector<Clause> QueryTree::getSuchThatOneVarTree() {
	return suchThatOneVarTree;
}

vector<Clause> QueryTree::getSuchThatTwoVarTree() {
	return suchThatTwoVarTree;
}

vector<Clause> QueryTree::getWithNoVarTree() {
	return withNoVarTree;
}

vector<Clause> QueryTree::getWithOneVarTree() {
	return withOneVarTree;
}

vector<Clause> QueryTree::getWithTwoVarTree() {
	return withTwoVarTree;
}

vector<Clause> QueryTree::getPatternOneVarTree() {
	return patternOneVarTree;
}

vector<Clause> QueryTree::getPatternTwoVarTree() {
	return patternTwoVarTree;
}

int QueryTree::getNumOfVar(vector<string> arrType) {
	int numOfVar = TWO_VARIABLE;
	for (auto &i : arrType) {
		if (i == "string" || i == "number") {
			--numOfVar;
		}
	}
	return numOfVar;
}