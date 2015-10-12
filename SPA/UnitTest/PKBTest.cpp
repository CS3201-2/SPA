#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(PKBTest)
	{
	public:
		TEST_METHOD_INITIALIZE(buildPKB) {
			DesignExtractor de = DesignExtractor();

			//set stmt list
			PKB::getPKBInstance()->addStmtToList(1, 0);
			PKB::getPKBInstance()->addStmtToList(2, 0);
			PKB::getPKBInstance()->addStmtToList(3, 2);
			PKB::getPKBInstance()->addStmtToList(4, 0);
			PKB::getPKBInstance()->addStmtToList(5, 0);
			PKB::getPKBInstance()->addStmtToList(6, 3);
			PKB::getPKBInstance()->addStmtToList(7, 0);
			PKB::getPKBInstance()->addStmtToList(8, 2);
			PKB::getPKBInstance()->addStmtToList(9, 0);
			PKB::getPKBInstance()->addStmtToList(10, 4);
			PKB::getPKBInstance()->addStmtToList(11, 0);
			PKB::getPKBInstance()->addStmtToList(12, 0);
			PKB::getPKBInstance()->addStmtToList(13, 0);
			PKB::getPKBInstance()->addStmtToList(14, 0);
			PKB::getPKBInstance()->addStmtToList(15, 0);
			PKB::getPKBInstance()->addStmtToList(16, 0);
			PKB::getPKBInstance()->addStmtToList(17, 0);
			
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
			PKB::getPKBInstance()->setPattern(7, "x", "x+2*y");
			PKB::getPKBInstance()->setPattern(8, "i", "i-1");
			PKB::getPKBInstance()->setPattern(9, "x", "_if_");
			PKB::getPKBInstance()->setPattern(10, "x", "x+1");
			PKB::getPKBInstance()->setPattern(11, "z", "1");
			PKB::getPKBInstance()->setPattern(10, "z", "z+x+i");
			PKB::getPKBInstance()->setPattern(11, "y", "z+2");
			PKB::getPKBInstance()->setPattern(12, "x", "x*y+z");
			PKB::getPKBInstance()->setPattern(13, "z", "5");
			PKB::getPKBInstance()->setPattern(14, "v", "z");

			//set calls
			PKB::getPKBInstance()->setCalls(-1, -2);
			PKB::getPKBInstance()->setCalls(-2, -3);

			//set callsStar
			de.setCallsStar();

			//reverse all the maps
			de.setReverseMap();
			PKB::getPKBInstance()->houseKeeping();
		}

		TEST_METHOD(testAllGetList) {
			//test getList functions
			list<int> test, expected;
			test = PKB::getPKBInstance()->getAssignList();
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(4);
			expected.push_back(5);
			expected.push_back(7);
			expected.push_back(9);
			expected.push_back(11);
			expected.push_back(12);
			expected.push_back(13);
			expected.push_back(14);
			expected.push_back(15);
			expected.push_back(16);
			expected.push_back(17);
			Assert::IsTrue(test == expected);
			
			test = PKB::getPKBInstance()->getProcList();
			expected.clear();
			expected.push_back(-1);
			expected.push_back(-2);
			expected.push_back(-3);
			Assert::IsTrue(test == expected);

			
			test = PKB::getPKBInstance()->getVarList();
			expected.clear();
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(3);
			expected.push_back(4);
			expected.push_back(5);
			Assert::IsTrue(test == expected);

			test = PKB::getPKBInstance()->getWhileList();
			expected.clear();
			expected.push_back(6);
			Assert::IsTrue(test == expected);

			test = PKB::getPKBInstance()->getCallList();
			expected.clear();
			expected.push_back(3);
			expected.push_back(8);
			Assert::IsTrue(test == expected);

			test = PKB::getPKBInstance()->getIfList();
			expected.clear();
			expected.push_back(10);
			Assert::IsTrue(test == expected);

			test = PKB::getPKBInstance()->getStmtList();
			expected.clear();
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(3);
			expected.push_back(4);
			expected.push_back(5);
			expected.push_back(6);
			expected.push_back(7);
			expected.push_back(8);
			expected.push_back(9);
			expected.push_back(10);
			expected.push_back(11);
			expected.push_back(12);
			expected.push_back(13);
			expected.push_back(14);
			expected.push_back(15);
			expected.push_back(16);
			expected.push_back(17);
			Assert::IsTrue(test == expected);

			test = PKB::getPKBInstance()->getParentList();
			expected.clear();
			expected.push_back(6);
			expected.push_back(10);
			Assert::IsTrue(test == expected);
			
			test = PKB::getPKBInstance()->getConstantList();
			expected.clear();
			expected.push_back(0);
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(3);
			expected.push_back(5);
			Assert::IsTrue(test == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getCallStmtProc(3) == -2);
			Assert::IsTrue(PKB::getPKBInstance()->getCallStmtProc(4) == 0);
		}

		TEST_METHOD(testVarTable) {
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("x") == 1);
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("y") == 4);
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("xy") == 0);
			Assert::IsTrue(PKB::getPKBInstance()->getVarName(3) == "i");
		}

		TEST_METHOD(testProcTable) {
			Assert::IsTrue(PKB::getPKBInstance()->getProcID("Third") == -3);
			Assert::IsTrue(PKB::getPKBInstance()->getProcID("Fourth") == 0);
			Assert::IsTrue(PKB::getPKBInstance()->getProcName(-1) == "First");
		}

		TEST_METHOD(testModifies) {
			list<int> test, expected;

			Assert::IsTrue(PKB::getPKBInstance()->getModifiesFirst(50) == list<int>());

			test = PKB::getPKBInstance()->getModifiesFirst(3);
			expected.push_back(-2);
			expected.push_back(-1);
			expected.push_back(3);
			expected.push_back(5);
			expected.push_back(6);
			expected.push_back(9);
			Assert::IsTrue(test == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getModifiesSecond(18) == list<int>());
			
			test = PKB::getPKBInstance()->getModifiesSecond(10);
			expected.clear();
			expected.push_back(1);
			expected.push_back(2);
			Assert::IsTrue(test == expected);

			Assert::IsFalse(PKB::getPKBInstance()->isModifiesValid(6, 7));
			Assert::IsTrue(PKB::getPKBInstance()->isModifiesValid(6, 5));
		}

		TEST_METHOD(testUses) {
			list<int> test, expected;

			Assert::IsTrue(PKB::getPKBInstance()->getUsesFirst(5) == list<int>());
		
			test = PKB::getPKBInstance()->getUsesFirst(3);
			expected.push_back(-2);
			expected.push_back(-1);
			expected.push_back(3);
			expected.push_back(6);
			expected.push_back(9);
			expected.push_back(13);
			Assert::IsTrue(test == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getUsesSecond(16) == list<int>());

			test = PKB::getPKBInstance()->getUsesSecond(17);
			expected.clear();
			expected.push_back(2);
			Assert::IsTrue(test == expected);

			Assert::IsTrue(PKB::getPKBInstance()->isUsesValid(-3, 2));
			Assert::IsFalse(PKB::getPKBInstance()->isUsesValid(9, 4));
		}

		TEST_METHOD(testFollows) {

		}
		TEST_METHOD(testFollowsStar) {

		}
		TEST_METHOD(testParent) {

		}
		TEST_METHOD(testParentStar) {

		}
		TEST_METHOD(testCalls) {

		}
		TEST_METHOD(testCallsStar) {

		}
		TEST_METHOD(testPattern) {

		}

	};
}