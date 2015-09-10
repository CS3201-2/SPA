#include <iostream>
#include <list>
using namespace std;

int main() {
	list<string> stmtList;
	stmtList.push_back("procedureSecond{");
	stmtList.push_back("x=0;");
	stmtList.push_back("i=5;");
	stmtList.push_back("whilei{");
	stmtList.push_back("x=x+y;");
	stmtList.push_back("i=i+1;}");
	stmtList.push_back("z=z+x+i;");
	stmtList.push_back("y=z+2;");
	stmtList.push_back("x=y+z;}");
	stmtList.push_back("procedureThrid{");
	stmtList.push_back("z=5;");
	stmtList.push_back("v=z;}");

	for (list<string>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		cout << *it;
		cout << endl;
	}

	cin.ignore();
}