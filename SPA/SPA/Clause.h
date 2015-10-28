#include <string>
#include <vector>

using namespace std;

class Clause {
private:
	string _relationship;
	vector<string> _var;
	vector<string> _varType;
	int index;

public:
	Clause(string, vector<string>, vector<string>);
	string getRelationShip();
	vector<string> getVar();
	vector<string> getVarType();
	void setIndex(int);
	int getIndex();
};