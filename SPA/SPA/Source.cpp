#include "CFG.h"

using namespace std;

int main()
{
	CFG cfg;
	list<Statement> expectedList;
	Statement temp = Statement();
	temp.setNumber(-1);
	temp.setContent("procedureFirst{");
	temp.setType(procDeclarationStmt);
	expectedList.push_back(temp);

	temp.setNumber(1);
	temp.setContent("x=2;");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(2);
	temp.setContent("z=3;");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(3);
	temp.setContent("callSecond;}");
	temp.setType(procCallStmt);
	expectedList.push_back(temp);

	temp.setNumber(-1);
	temp.setContent("procedureSecond{");
	temp.setType(procDeclarationStmt);
	expectedList.push_back(temp);

	temp.setNumber(4);
	temp.setContent("x=0;");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(5);
	temp.setContent("i=5;");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(6);
	temp.setContent("whilei{");
	temp.setType(whileStmt);
	expectedList.push_back(temp);

	temp.setNumber(7);
	temp.setContent("x=(x+2)*y;");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(8);
	temp.setContent("callThird;");
	temp.setType(procCallStmt);
	expectedList.push_back(temp);

	temp.setNumber(9);
	temp.setContent("i=i-1;}");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(10);
	temp.setContent("ifxthen{");
	temp.setType(ifStmt);
	expectedList.push_back(temp);

	temp.setNumber(11);
	temp.setContent("x=x+1;}");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(10);
	temp.setContent("ifxthen{");
	temp.setType(elseStmt);
	expectedList.push_back(temp);

	temp.setNumber(12);
	temp.setContent("z=1;}");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(13);
	temp.setContent("z=z+(x+1);");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(14);
	temp.setContent("y=z+2;");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(15);
	temp.setContent("x=(x*y)+z;}");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(-1);
	temp.setContent("procedureThird{");
	temp.setType(procDeclarationStmt);
	expectedList.push_back(temp);

	temp.setNumber(16);
	temp.setContent("z=5;");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	temp.setNumber(17);
	temp.setContent("v=z;}");
	temp.setType(assignmentStmt);
	expectedList.push_back(temp);

	cfg.buildGraph(expectedList);
	cfg.printNextTableWithDummy();
	cfg.printDummyForNext();

	cin.ignore();
	return 0;
}