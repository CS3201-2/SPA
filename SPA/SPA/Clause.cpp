#include "Clause.h"


Clause::Clause(string relationship, vector<string> var, vector<string> varType) {
	_relationship = relationship;
	_var = var;
	_varType = varType;
}

string Clause::getRelationship() {
	return _relationship;
}

vector<string> Clause::getVar() {
	return _var;
}

vector<string> Clause::getVarType() {
	return _varType;
}

void Clause::setIndex(int index) {
	_index = index;
}

int Clause::getIndex() {
	return _index;
}

