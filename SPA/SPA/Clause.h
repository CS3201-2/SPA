#include <string>
#include <vector>

using namespace std;

#ifndef Clause_H
#define Clause_H
class Clause {
private:
	string _relationship;
	vector<string> _var;
	vector<string> _varType;
	int _index;

public:
	Clause(string, vector<string>, vector<string>);
	string getRelationship();
	vector<string> getVar();
	vector<string> getVarType();
	void setIndex(int);
	int getIndex();
};

#endif