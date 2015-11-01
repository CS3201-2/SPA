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
	int _numOfVar;

public:
	Clause();
	Clause(string, vector<string>, vector<string>);
	Clause(string, vector<string>, vector<string>, int);
	string getRelationship();
	vector<string> getVar();
	vector<string> getVarType();
	void setIndex(int);
	int getIndex();
	int getNumOfVar();
};

#endif