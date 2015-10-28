#include "QueryTree.h"
using namespace std;

const string NO_RELATIONSHIP = "";
const int NO_VARIABLE = 0;
const int ONE_VARIABLE = 1;
const int TWO_VARIABLE = 2;

QueryTree::QueryTree() {

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

int QueryTree::countNumOfVar(vector<string> arrType) {
	int count = 2;
	for (auto &i : arrType) {
		if (i == "string" || i == "variable") {
			
		}
	}
}