#include "Clause.h"


Clause::Clause(string relationship, vector<string> var, vector<string> varType) {
	_relationship = relationship;
	_var = var;
	_varType = varType;
}

