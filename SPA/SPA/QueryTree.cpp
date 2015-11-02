#include "QueryTree.h"
using namespace std;

const string NO_RELATIONSHIP = "";
const string SELECT_RELATIONSHIP = "select";
const int NO_VARIABLE = 0;
const int ONE_VARIABLE = 1;
const int TWO_VARIABLE = 2;
const int FIRST_TYPE = 0;
const string VARIABLE_TYPE = "variable";
const string BOOLEAN_TYPE = "boolean";
const string STRING_TYPE = "string";
const string NUMBER_TYPE = "number";
const string ALL_TYPE = "all";
const bool IS_NOT_USEFUL = false;
const bool IS_USEFUL = true;

QueryTree::QueryTree() {

}

//Inserting the particular type of query into its respective tree
void QueryTree::insertSuchThat(string rel, vector<string> arrVar, vector<string> arrType) {
	int numOfVar = getNumOfVar(arrType);
	Clause clause = Clause(rel, arrVar, arrType, numOfVar);
	_allClauses.push_back(clause);
}

void QueryTree::insertPattern(string syn, string synType, vector<string> arrPtrn, vector<string> ptrnType) {
	arrPtrn.push_back(syn);
	ptrnType.push_back(synType);
	if (ptrnType.at(FIRST_TYPE) == VARIABLE_TYPE) {
		Clause clause = Clause(synType, arrPtrn, ptrnType, TWO_VARIABLE);
		_allClauses.push_back(clause);
	}
	else {
		Clause clause = Clause(synType, arrPtrn, ptrnType, ONE_VARIABLE);
		_allClauses.push_back(clause);
	}
}

void QueryTree::insertVariable(string variable, string variableType) {
	vector<string> varVector;
	varVector.push_back(variable);
	vector<string> varTypeVector;
	varTypeVector.push_back(variableType);
	Clause clause = Clause(NO_RELATIONSHIP, varVector, varTypeVector);
	_variableTree.push_back(clause);
}

void QueryTree::insertSelect(vector<string> var, vector<string> varType) {
	_selectClause = Clause(SELECT_RELATIONSHIP, var, varType);
}

void QueryTree::insertSelect(string var, string varType) {
	vector<string> varVector;
	varVector.push_back(var);
	vector<string> varTypeVector;
	varTypeVector.push_back(varType);
	_selectClause = Clause(SELECT_RELATIONSHIP, varVector, varTypeVector);
}

void QueryTree::insertWith(string rel, vector<string> arrWith, vector<string> withType) {
	int numOfVar = getNumOfVar(withType);
	Clause clause = Clause(rel, arrWith, withType, numOfVar);
	_allClauses.push_back(clause);
}

vector<Clause> QueryTree::getVariableTree() {
	return _variableTree;
}

Clause QueryTree::getSelectClause() {
	return _selectClause;
}

vector<Clause> QueryTree::getUsefulNoVarTree() {
	return _usefulNoVarTree;
}

vector<Clause> QueryTree::getUsefulOneVarTree() {
	return _usefulOneVarTree;
}

vector<Clause> QueryTree::getUsefulTwoVarTree() {
	return _usefulTwoVarTree;
}

vector<Clause> QueryTree::getUselessOneVarTree() {
	return _uselessOneVarTree;
}

vector<Clause> QueryTree::getUselessTwoVarTree() {
	return _uselessTwoVarTree;
}

int QueryTree::getNumOfVar(vector<string> arrType) {
	int numOfVar = TWO_VARIABLE;
	for (auto &i : arrType) {
		if (i == STRING_TYPE || i == NUMBER_TYPE || i == ALL_TYPE) {
			--numOfVar;
		}
	}
	return numOfVar;
}

void QueryTree::grouping() {
	for (vector<Clause>::iterator it = _allClauses.begin(); it != _allClauses.end();) {
		if ((*it).getNumOfVar() == NO_VARIABLE) {
			_usefulNoVarTree.push_back(*it);
			it = _allClauses.erase(it);
		}
		else {
			++it;
		}
	}

	if (_selectClause.getVarType().at(FIRST_TYPE) == BOOLEAN_TYPE) {
		for (size_t i = 0; i < _allClauses.size(); ++i) {
			if (_allClauses.at(i).getNumOfVar() == ONE_VARIABLE) {
				_usefulOneVarTree.push_back(_allClauses.at(i));
			}
			else {
				_usefulTwoVarTree.push_back(_allClauses.at(i));
			}
		}
	}
	else {
		vector<vector<string>> queryVars;
		for (size_t i = 0; i < _allClauses.size(); ++i) {
			_useful.push_back(IS_NOT_USEFUL);
			vector<string> var = _allClauses.at(i).getVar();
			vector<string> varType = _allClauses.at(i).getVarType();
			vector<string> temp;
			for (size_t j = 0; j < var.size(); ++j) {
				if (varType.at(j) != STRING_TYPE && varType.at(j) != NUMBER_TYPE
					&& varType.at(j) != ALL_TYPE) {
					temp.push_back(var.at(j));
				}
			}
			queryVars.push_back(temp);
		}
		
		vector<string> _usefulVars = _selectClause.getVar();
		for (size_t i = 0; i < _allClauses.size(); ++i) {
			vector<string> temp1 = _allClauses.at(i).getVar();
			if (hasCommon(_usefulVars, temp1)) {
				_useful[i] = IS_USEFUL;
				_usefulVars.insert(_usefulVars.end(), temp1.begin(), temp1.end());
			}
			for (size_t j = 0; j < i; ++j) {
				temp1 = _allClauses.at(j).getVar();
				if (_useful.at(j) != IS_USEFUL &&
					hasCommon(_usefulVars, temp1)) {
					_useful[j] = IS_USEFUL;
					_usefulVars.insert(_usefulVars.end(), temp1.begin(), temp1.end());
				}
			}
		}

		for (size_t i = 0; i < _allClauses.size(); ++i) {
			if (_useful[i] == IS_USEFUL) {
				if (_allClauses.at(i).getNumOfVar() == ONE_VARIABLE) {
					_usefulOneVarTree.push_back(_allClauses.at(i));
				}
				else {
					_usefulTwoVarTree.push_back(_allClauses.at(i));
				}
			}
			else {
				if (_allClauses.at(i).getNumOfVar() == ONE_VARIABLE) {
					_uselessOneVarTree.push_back(_allClauses.at(i));
				}
				else {
					_uselessOneVarTree.push_back(_allClauses.at(i));
				}
			}
		}
	}
}

bool QueryTree::hasCommon(vector<string> v1, vector<string> v2) {
	for (auto &i : v1) {
		for (auto &j : v2) {
			if (i == j) {
				return true;
			}
		}
	}

	return false;
}