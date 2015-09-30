#include <string>
#include <map>
#include <list>

using namespace std;

#ifndef Modifies_H
#define Modifies_H

class Modifies
{
public:
	Modifies();
    
	void setModifies();
	list<int> getModifiesFirst(int);
	list<int> getModifiesSecond(int);
	bool isModifiesValid(int, int);
	void printAllModifies();


	list<int> getModifiesLine( int );

	void setModifiesStmt( int, int );
	// return list of var_id modified by stmt number
	list<int> getModifiesVar(int stmt_number);
	void printMap();

	void sortMap();
private:
	map<int,std::list<int>> modifiesMap;
};

#endif
