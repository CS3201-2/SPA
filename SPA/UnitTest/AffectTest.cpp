#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Affect.h"
#include "DesignExtractor.h"
#include "SPALog.h"
const string EXCEPT_BOUND = "Exception: index out of bound!";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(AffectTest)
	{
	public:
		TEST_METHOD_INITIALIZE(buildPKB) {
			DesignExtractor de = DesignExtractor();
			//set CFG
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
			PKB::getPKBInstance()->buildCFG(expectedList);
			//set stmt list
			PKB::getPKBInstance()->addStmtToList(1, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(2, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(3, procCallStmt);
			PKB::getPKBInstance()->addStmtToList(4, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(5, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(6, whileStmt);
			PKB::getPKBInstance()->addStmtToList(7, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(8, procCallStmt);
			PKB::getPKBInstance()->addStmtToList(9, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(10, ifStmt);
			PKB::getPKBInstance()->addStmtToList(11, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(12, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(13, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(14, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(15, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(16, assignmentStmt);
			PKB::getPKBInstance()->addStmtToList(17, assignmentStmt);

			//set constant list
			PKB::getPKBInstance()->addConstantToList(2);
			PKB::getPKBInstance()->addConstantToList(3);
			PKB::getPKBInstance()->addConstantToList(0);
			PKB::getPKBInstance()->addConstantToList(5);
			PKB::getPKBInstance()->addConstantToList(2);
			PKB::getPKBInstance()->addConstantToList(1);
			PKB::getPKBInstance()->addConstantToList(1);
			PKB::getPKBInstance()->addConstantToList(2);
			PKB::getPKBInstance()->addConstantToList(5);

			//set callStmtMap
			PKB::getPKBInstance()->addToCallStmtProcMap(3, -2);
			PKB::getPKBInstance()->addToCallStmtProcMap(8, -3);

			//set procTable
			PKB::getPKBInstance()->insertProc("First");
			PKB::getPKBInstance()->insertProc("Second");
			PKB::getPKBInstance()->insertProc("Third");

			//set varTable
			PKB::getPKBInstance()->insertVar("x");
			PKB::getPKBInstance()->insertVar("z");
			PKB::getPKBInstance()->insertVar("i");
			PKB::getPKBInstance()->insertVar("y");
			PKB::getPKBInstance()->insertVar("v");

			//set modifies
			list<int> varList;
			varList.push_back(4);
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(-3, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(4);
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(-2, varList);
			varList.clear();

			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(4);
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(-1, varList);

			varList.clear();
			varList.push_back(1);
			PKB::getPKBInstance()->setModifies(1, varList);

			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setModifies(2, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(4);
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(3, varList);

			varList.clear();
			varList.push_back(1);
			PKB::getPKBInstance()->setModifies(4, varList);

			varList.clear();
			varList.push_back(3);
			PKB::getPKBInstance()->setModifies(5, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(6, varList);

			varList.clear();
			varList.push_back(1);
			PKB::getPKBInstance()->setModifies(7, varList);

			varList.clear();
			varList.push_back(2);
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(8, varList);

			varList.clear();
			varList.push_back(3);
			PKB::getPKBInstance()->setModifies(9, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			PKB::getPKBInstance()->setModifies(10, varList);

			varList.clear();
			varList.push_back(1);
			PKB::getPKBInstance()->setModifies(11, varList);

			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setModifies(12, varList);

			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setModifies(13, varList);

			varList.clear();
			varList.push_back(4);
			PKB::getPKBInstance()->setModifies(14, varList);

			varList.clear();
			varList.push_back(1);
			PKB::getPKBInstance()->setModifies(15, varList);

			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setModifies(16, varList);

			varList.clear();
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(17, varList);

			//set uses
			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setUses(-3, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(4);
			PKB::getPKBInstance()->setUses(-2, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(4);
			PKB::getPKBInstance()->setUses(-1, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(4);
			PKB::getPKBInstance()->setUses(3, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			varList.push_back(4);
			PKB::getPKBInstance()->setUses(6, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(4);
			PKB::getPKBInstance()->setUses(7, varList);

			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setUses(8, varList);

			varList.clear();
			varList.push_back(3);
			PKB::getPKBInstance()->setUses(9, varList);

			varList.clear();
			varList.push_back(1);
			PKB::getPKBInstance()->setUses(10, varList);

			varList.clear();
			varList.push_back(1);
			PKB::getPKBInstance()->setUses(11, varList);
			varList.clear();

			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(3);
			PKB::getPKBInstance()->setUses(13, varList);

			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setUses(14, varList);

			varList.clear();
			varList.push_back(1);
			varList.push_back(2);
			varList.push_back(4);
			PKB::getPKBInstance()->setUses(15, varList);

			varList.clear();
			varList.push_back(2);
			PKB::getPKBInstance()->setUses(17, varList);

			//set follows
			PKB::getPKBInstance()->setFollows(1, 2);
			PKB::getPKBInstance()->setFollows(2, 3);
			PKB::getPKBInstance()->setFollows(4, 5);
			PKB::getPKBInstance()->setFollows(5, 6);
			PKB::getPKBInstance()->setFollows(6, 10);
			PKB::getPKBInstance()->setFollows(7, 8);
			PKB::getPKBInstance()->setFollows(8, 9);
			PKB::getPKBInstance()->setFollows(10, 13);
			PKB::getPKBInstance()->setFollows(13, 14);
			PKB::getPKBInstance()->setFollows(14, 15);
			PKB::getPKBInstance()->setFollows(16, 17);

			//set followsStar
			de.setFollowsStar();

			//set parent
			list<int> stmtList;
			stmtList.push_back(7);
			stmtList.push_back(8);
			stmtList.push_back(9);
			PKB::getPKBInstance()->setParent(6, stmtList);
			stmtList.clear();

			stmtList.push_back(11);
			stmtList.push_back(12);
			PKB::getPKBInstance()->setParent(10, stmtList);
			stmtList.clear();

			//set parentStar
			de.setParentStar();

			//set pattern
			PKB::getPKBInstance()->setPattern(1, "x", "2");
			PKB::getPKBInstance()->setPattern(2, "z", "3");
			PKB::getPKBInstance()->setPattern(4, "x", "0");
			PKB::getPKBInstance()->setPattern(5, "i", "5");
			PKB::getPKBInstance()->setPattern(6, "i", "_while_");
			PKB::getPKBInstance()->setPattern(7, "x", "(x+2)*y");
			PKB::getPKBInstance()->setPattern(9, "i", "i-1");
			PKB::getPKBInstance()->setPattern(10, "x", "_if_");
			PKB::getPKBInstance()->setPattern(11, "x", "x+1");
			PKB::getPKBInstance()->setPattern(12, "z", "1");
			PKB::getPKBInstance()->setPattern(13, "z", "x+(x+1)");
			PKB::getPKBInstance()->setPattern(14, "y", "z+2");
			PKB::getPKBInstance()->setPattern(15, "x", "(x*y)+z");
			PKB::getPKBInstance()->setPattern(16, "z", "5");
			PKB::getPKBInstance()->setPattern(17, "v", "z");

			//set calls
			PKB::getPKBInstance()->setCalls(-1, -2);
			PKB::getPKBInstance()->setCalls(-2, -3);

			//set callsStar
			de.setCallsStar();

			//reverse all the maps
			de.setReverseMap();
			PKB::getPKBInstance()->houseKeeping();
		}

		TEST_METHOD_CLEANUP(clean) {
			PKB::DestroyInstance();
		}

		TEST_METHOD(testAffectValid)
		{
			Affect af;
			Assert::IsTrue(af.isAffectValid(4, 7));
			Assert::IsTrue(af.isAffectValid(5,9));
			Assert::IsTrue(af.isAffectValid(12, 13));
			Assert::IsFalse(af.isAffectValid(6, 7));
			Assert::IsFalse(af.isAffectValid(5, 7));
		}
	};
}