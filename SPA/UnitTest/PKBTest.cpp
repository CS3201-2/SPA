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

		TEST_METHOD_CLEANUP(clean) {
			PKB::DestroyInstance();
		}

		TEST_METHOD(testAllGetListFunctions) {
			//test getList functions
			list<int> actual, expected;
			actual = PKB::getPKBInstance()->getAssignList();
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
			Assert::IsTrue(actual == expected);
			
			actual = PKB::getPKBInstance()->getProcList();
			expected.clear();
			expected.push_back(-1);
			expected.push_back(-2);
			expected.push_back(-3);
			Assert::IsTrue(actual == expected);

			
			actual = PKB::getPKBInstance()->getVarList();
			expected.clear();
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(3);
			expected.push_back(4);
			expected.push_back(5);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getWhileList();
			expected.clear();
			expected.push_back(6);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getCallList();
			expected.clear();
			expected.push_back(3);
			expected.push_back(8);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getIfList();
			expected.clear();
			expected.push_back(10);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getStmtList();
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
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getParentList();
			expected.clear();
			expected.push_back(6);
			expected.push_back(10);
			Assert::IsTrue(actual == expected);
			
			actual = PKB::getPKBInstance()->getConstantList();
			expected.clear();
			expected.push_back(0);
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(3);
			expected.push_back(5);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getCallStmtProc(3) == -2);
			Assert::IsTrue(PKB::getPKBInstance()->getCallStmtProc(4) == 0);
		}

		TEST_METHOD(testVarTableFunctions) {
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("x") == 1);
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("y") == 4);
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("xy") == 0);
			Assert::IsTrue(PKB::getPKBInstance()->getVarName(3) == "i");
		}

		TEST_METHOD(testProcTableFunctions) {
			Assert::IsTrue(PKB::getPKBInstance()->getProcID("Third") == -3);
			Assert::IsTrue(PKB::getPKBInstance()->getProcID("Fourth") == 0);
			Assert::IsTrue(PKB::getPKBInstance()->getProcName(-1) == "First");
		}

		TEST_METHOD(testModifiesFunctions) {
			list<int> actual, expected;

			Assert::IsTrue(PKB::getPKBInstance()->getModifiesFirst(50) == list<int>());

			//test getModifiesFirst
			actual = PKB::getPKBInstance()->getModifiesFirst(3);
			expected.push_back(-2);
			expected.push_back(-1);
			expected.push_back(3);
			expected.push_back(5);
			expected.push_back(6);
			expected.push_back(9);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getModifiesFirst(1);
			expected.clear();
			expected.push_back(-2);
			expected.push_back(-1);
			expected.push_back(1);
			expected.push_back(3);
			expected.push_back(4);
			expected.push_back(6);
			expected.push_back(7);
			expected.push_back(10);
			expected.push_back(11);
			expected.push_back(15);
			Assert::IsTrue(actual == expected);


			//test getModifiesSecond
			Assert::IsTrue(PKB::getPKBInstance()->getModifiesSecond(18) == list<int>());
			
			actual = PKB::getPKBInstance()->getModifiesSecond(10);
			expected.clear();
			expected.push_back(1);
			expected.push_back(2);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getModifiesSecond(6);
			expected.clear();
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(3);
			expected.push_back(5);
			Assert::IsTrue(actual == expected);


			//test isModifiesValid
			Assert::IsFalse(PKB::getPKBInstance()->isModifiesValid(6, 7));
			Assert::IsFalse(PKB::getPKBInstance()->isModifiesValid(19, 7));
			Assert::IsTrue(PKB::getPKBInstance()->isModifiesValid(6, 5));
			Assert::IsTrue(PKB::getPKBInstance()->isModifiesValid(15, 1));
		}

		TEST_METHOD(testUsesFunctions) {
			list<int> actual, expected;
				
			//test getUsesFirst
			Assert::IsTrue(PKB::getPKBInstance()->getUsesFirst(5) == list<int>());
		
			actual = PKB::getPKBInstance()->getUsesFirst(3);
			expected.push_back(-2);
			expected.push_back(-1);
			expected.push_back(3);
			expected.push_back(6);
			expected.push_back(9);
			expected.push_back(13);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getUsesFirst(4);
			expected.clear();
			expected.push_back(-2);
			expected.push_back(-1);
			expected.push_back(3);
			expected.push_back(6);
			expected.push_back(7);
			expected.push_back(15);
			Assert::IsTrue(actual == expected);


			//test getUsesSecond
			Assert::IsTrue(PKB::getPKBInstance()->getUsesSecond(16) == list<int>());

			actual = PKB::getPKBInstance()->getUsesSecond(17);
			expected.clear();
			expected.push_back(2);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getUsesSecond(6);
			expected.clear();
			expected.push_back(1);
			expected.push_back(2);
			expected.push_back(3);
			expected.push_back(4);
			Assert::IsTrue(actual == expected);


			//test isUsesValid
			Assert::IsTrue(PKB::getPKBInstance()->isUsesValid(-3, 2));
			Assert::IsFalse(PKB::getPKBInstance()->isUsesValid(9, 4));
			Assert::IsFalse(PKB::getPKBInstance()->isUsesValid(19, 4));
			Assert::IsTrue(PKB::getPKBInstance()->isUsesValid(13, 3));
		}

		TEST_METHOD(testFollowsFunctions) {
			int actual, expected;

			//test getFollowsFirst
			actual = PKB::getPKBInstance()->getFollowsFirst(3);
			expected = 2;
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getFollowsFirst(10);
			expected = 6;
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getFollowsFirst(7) == 0);
		
			
			//test getFollowsSecond
			actual = PKB::getPKBInstance()->getFollowsSecond(4);
			expected = 5;
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getFollowsSecond(6);
			expected = 10;
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getFollowsSecond(18) == 0);
		
			
			//test isFollowsValid
			Assert::IsTrue(PKB::getPKBInstance()->isFollowsValid(2, 3));
			Assert::IsTrue(PKB::getPKBInstance()->isFollowsValid(10, 13));
			Assert::IsFalse(PKB::getPKBInstance()->isFollowsValid(13, 10));
			Assert::IsFalse(PKB::getPKBInstance()->isFollowsValid(17, 2));
		}

		TEST_METHOD(testFollowsStarFunctions) {
			list<int> actual, expected;

			//test getFollowsStarFirst
			actual = PKB::getPKBInstance()->getFollowsStarFirst(10);
			expected.push_back(4);
			expected.push_back(5);
			expected.push_back(6);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getFollowsStarFirst(6);
			expected.clear();
			expected.push_back(4);
			expected.push_back(5);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getFollowsStarFirst(16) == list<int>());
		
			
			//test getFollowsStarSecond
			actual = PKB::getPKBInstance()->getFollowsStarSecond(10);
			expected.clear();
			expected.push_back(13);
			expected.push_back(14);
			expected.push_back(15);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getFollowsStarSecond(4);
			expected.clear();
			expected.push_back(5);
			expected.push_back(6);
			expected.push_back(10);
			expected.push_back(13);
			expected.push_back(14);
			expected.push_back(15);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getFollowsStarSecond(3) == list<int>());
		
			
			//test isFollowsStarValid
			Assert::IsTrue(PKB::getPKBInstance()->isFollowsStarValid(4, 15));
			Assert::IsTrue(PKB::getPKBInstance()->isFollowsStarValid(6, 14));
			Assert::IsFalse(PKB::getPKBInstance()->isFollowsStarValid(8, 10));
			Assert::IsFalse(PKB::getPKBInstance()->isFollowsStarValid(19, 10));
		}

		TEST_METHOD(testParentFunctions) {
			list<int> actualList, expectedList;
			int actualInt, expectedInt;

			//test getParentFirst
			actualInt = PKB::getPKBInstance()->getParentFirst(9);
			expectedInt = 6;
			Assert::IsTrue(actualInt == expectedInt);

			actualInt = PKB::getPKBInstance()->getParentFirst(12);
			expectedInt = 10;
			Assert::IsTrue(actualInt == expectedInt);

			Assert::IsTrue(PKB::getPKBInstance()->getParentFirst(13) == 0);
		
			
			//test getParentSecond
			actualList = PKB::getPKBInstance()->getParentSecond(6);
			expectedList.push_back(7);
			expectedList.push_back(8);
			expectedList.push_back(9);
			Assert::IsTrue(actualList == expectedList);

			actualList = PKB::getPKBInstance()->getParentSecond(10);
			expectedList.clear();
			expectedList.push_back(11);
			expectedList.push_back(12);
			Assert::IsTrue(actualList == expectedList);

			Assert::IsTrue(PKB::getPKBInstance()->getParentSecond(11) == list<int>());
		
			
			//test isParentValid
			Assert::IsTrue(PKB::getPKBInstance()->isParentValid(6, 9));
			Assert::IsTrue(PKB::getPKBInstance()->isParentValid(10, 12));
			Assert::IsFalse(PKB::getPKBInstance()->isParentValid(6, 10));
			Assert::IsFalse(PKB::getPKBInstance()->isParentValid(20, 10));
		}

		TEST_METHOD(testParentStarFunctions) {
			list<int> actual, expected;

			//test getParentStarFirst
			actual = PKB::getPKBInstance()->getParentStarFirst(9);
			expected.push_back(6);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getParentStarFirst(12);
			expected.clear();
			expected.push_back(10);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getParentStarFirst(13) == list<int>());


			//test getParentStarSecond
			actual = PKB::getPKBInstance()->getParentStarSecond(6);
			expected.clear();
			expected.push_back(7);
			expected.push_back(8);
			expected.push_back(9);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getParentStarSecond(10);
			expected.clear();
			expected.push_back(11);
			expected.push_back(12);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getParentStarSecond(11) == list<int>());


			//test isParentStarValid
			Assert::IsTrue(PKB::getPKBInstance()->isParentStarValid(6, 9));
			Assert::IsTrue(PKB::getPKBInstance()->isParentStarValid(10, 11));
			Assert::IsFalse(PKB::getPKBInstance()->isParentStarValid(10, 13));
			Assert::IsFalse(PKB::getPKBInstance()->isParentStarValid(20, 23));
		}

		TEST_METHOD(testCallsFunctions) {
			list<int> actual, expected;

			//test getCallsFirst
			actual = PKB::getPKBInstance()->getCallsFirst(-3);
			expected.push_back(-2);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getCallsFirst(-2);
			expected.clear();
			expected.push_back(-1);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getCallsFirst(-1) == list<int>());
		
			
			//test getCallsSecond
			actual = PKB::getPKBInstance()->getCallsSecond(-1);
			expected.clear();
			expected.push_back(-2);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getCallsSecond(-3) == list<int>());


			//test isCallsValid
			Assert::IsTrue(PKB::getPKBInstance()->isCallsValid(-1, -2));
			Assert::IsTrue(PKB::getPKBInstance()->isCallsValid(-2, -3));
			Assert::IsFalse(PKB::getPKBInstance()->isCallsValid(-2, -1));
			Assert::IsFalse(PKB::getPKBInstance()->isCallsValid(-4, -1));
		}

		TEST_METHOD(testCallsStarFunctions) {
			list<int> actual, expected;

			//test getCallsStarFirst
			actual = PKB::getPKBInstance()->getCallsStarFirst(-3);
			expected.push_back(-2);
			expected.push_back(-1);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getCallsStarFirst(-2);
			expected.clear();
			expected.push_back(-1);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getCallsStarFirst(-1) == list<int>());


			//test getCallsStarSecond
			actual = PKB::getPKBInstance()->getCallsStarSecond(-1);
			expected.clear();
			expected.push_back(-3);
			expected.push_back(-2);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getCallsStarSecond(-2);
			expected.clear();
			expected.push_back(-3);
			Assert::IsTrue(actual == expected);

			Assert::IsTrue(PKB::getPKBInstance()->getCallsStarSecond(-3) == list<int>());


			//test isCallsStarValid
			Assert::IsTrue(PKB::getPKBInstance()->isCallsStarValid(-1, -2));
			Assert::IsTrue(PKB::getPKBInstance()->isCallsStarValid(-1, -3));
			Assert::IsFalse(PKB::getPKBInstance()->isCallsStarValid(-2, -1));
			Assert::IsFalse(PKB::getPKBInstance()->isCallsStarValid(-4, -2));
		}

		TEST_METHOD(testPatternFunctions) {

		}
	};
}