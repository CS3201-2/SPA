#include "QueryTree.h"
using namespace std;

const string NO_RELATIONSHIP = "";
const int VARIABLE_TREE = 0;
const int SELECT_TREE = 1;
const int ST_NO_VAR_TREE = 2;
const int ST_ONE_VAR_TREE = 3;
const int ST_TWO_VAR_TREE = 4;
const int WITH_NO_VAR_TREE = 5;
const int WITH_ONE_VAR_TREE = 6;
const int WITH_TWO_VAR_TREE = 7;
const int PATTERN_ONE_VAR_TREE = 8;
const int PATTERN_TWO_VAR_TREE = 9;

QueryTree::QueryTree() {
	qTree.push_back(variableTree);
	qTree.push_back(selectTree);
	qTree.push_back(suchThatNoVarTree);
	qTree.push_back(suchThatOneVarTree);
	qTree.push_back(suchThatTwoVarTree);
	qTree.push_back(withNoVarTree);
	qTree.push_back(withOneVarTree);
	qTree.push_back(withTwoVarTree);
	qTree.push_back(patternOneVarTree);
	qTree.push_back(patternTwoVarTree);
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


