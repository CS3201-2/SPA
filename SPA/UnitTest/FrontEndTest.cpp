#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
#include "Checker.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(frontEndTest)
	{
	public: 
		TEST_METHOD_INITIALIZE(buildPKB) {
			string str;
			str = "procedure First{\n";
			str += "  x=  2;\n";
			str += "  z =3;\n";
			str += "  call Second;}\n";
			str += "procedure Second{\n";
			str += "  x = 0;\n";
			str += "  i = 5;\n";
			str += "  while i{\n";
			str += "     x = (x + 2) * y;\n";
			str += "     call Third;\n";
			str += "     i = i - 1; }\n";
			str += "  if x then{\n";
			str += "     x = x + 1; }\n";
			str += "  else {\n";
			str += "     z = 1;\n";
			str += "  }\n";
			str += "  z = z + (x + 1);\n";
			str += "  y = z + 2;\n";
			str += "  x = (x*y) + z; }\n";
			str += "procedure Third{\n";
			str += "  z = 5;\n";
			str += "v = z; }\n";

			Parser parser = Parser();
			Checker checker;
			list<Statement> stmtList = parser.prepareSourceList(str);

			if (!checker.isSyntaxCorrect(stmtList)) {

			}
			else {
				parser.parseSource(stmtList);
				DesignExtractor de = DesignExtractor();
				de.setFollowsStar();
				de.setParentStar();
				de.resetModifies();
				de.resetUses();
				de.setReverseMap();
			}
		}

		TEST_METHOD_CLEANUP(clean) {
			PKB::DestroyInstance();
		}

		TEST_METHOD(testAllGetListFunctionsIntegration) {
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

		TEST_METHOD(testVarTableFunctionsIntegration) {
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("x") == 1);
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("y") == 4);
			Assert::IsTrue(PKB::getPKBInstance()->getVarID("xy") == 0);
			Assert::IsTrue(PKB::getPKBInstance()->getVarName(3) == "i");
		}

		TEST_METHOD(testProcTableFunctionsIntegration) {
			Assert::IsTrue(PKB::getPKBInstance()->getProcID("Third") == -3);
			Assert::IsTrue(PKB::getPKBInstance()->getProcID("Fourth") == 0);
			Assert::IsTrue(PKB::getPKBInstance()->getProcName(-1) == "First");
		}

		TEST_METHOD(testModifiesFunctionsIntegration) {
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

		TEST_METHOD(testUsesFunctionsIntegration) {
			list<int> actual, expected;

			//test getUsesFirst
			Assert::IsTrue(PKB::getPKBInstance()->getUsesFirst(5) == list<int>());

			actual = PKB::getPKBInstance()->getUsesFirst(3);
			expected.push_back(-2);
			expected.push_back(-1);
			expected.push_back(3);
			expected.push_back(6);
			expected.push_back(9);
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
			Assert::IsTrue(PKB::getPKBInstance()->isUsesValid(13, 1));
		}

		TEST_METHOD(testFollowsFunctionsIntegration) {
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

		TEST_METHOD(testFollowsStarFunctionsIntegration) {
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

		TEST_METHOD(testParentFunctionsIntegration) {
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

		TEST_METHOD(testParentStarFunctionsIntegration) {
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

		TEST_METHOD(testCallsFunctionsIntegration) {
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

		TEST_METHOD(testCallsStarFunctionsIntegration) {
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

		TEST_METHOD(testPatternFunctionsIntegration) {
			list<int> actual, expected;

			//for pattern a("x", _)
			actual = PKB::getPKBInstance()->getAssignWithFirstExact("x");
			expected.push_back(1);
			expected.push_back(4);
			expected.push_back(7);
			expected.push_back(11);
			expected.push_back(15);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithFirstExact("z");
			expected.clear();
			expected.push_back(2);
			expected.push_back(12);
			expected.push_back(13);
			expected.push_back(16);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithFirstExact("abc");
			Assert::IsTrue(actual == list<int>());


			//for pattern a(_, "x")
			actual = PKB::getPKBInstance()->getAssignWithSecondExact("x+1");
			expected.clear();
			expected.push_back(11);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithSecondExact("(x+1)");
			expected.clear();
			expected.push_back(11);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithSecondExact("x*y");
			Assert::IsTrue(actual == list<int>());


			//for pattern a(_, _"x"_)
			actual = PKB::getPKBInstance()->getAssignWithSecond("x+1");
			expected.clear();
			expected.push_back(11);
			expected.push_back(13);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithSecond("(x+1)");
			expected.clear();
			expected.push_back(11);
			expected.push_back(13);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithSecond("z");
			expected.clear();
			expected.push_back(13);
			expected.push_back(14);
			expected.push_back(15);
			expected.push_back(17);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithSecond("z+(x+1)");
			expected.clear();
			expected.push_back(13);
			Assert::IsTrue(actual == expected);


			//for pattern a("x", "y")
			actual = PKB::getPKBInstance()->getAssignWithBothExact("x", "x+1");
			expected.clear();
			expected.push_back(11);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithBothExact("z", "z+(x+1)");
			expected.clear();
			expected.push_back(13);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithBothExact("z", "5");
			expected.clear();
			expected.push_back(16);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithBothExact("x", "x");
			Assert::IsTrue(actual == list<int>());

			
			//for pattern a("x", _"y"_)
			actual = PKB::getPKBInstance()->getAssignWithBoth("x", "y");
			expected.clear();
			expected.push_back(7);
			expected.push_back(15);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithBoth("z", "x+1");
			expected.clear();
			expected.push_back(13);
			Assert::IsTrue(actual == expected);

			actual = PKB::getPKBInstance()->getAssignWithBoth("z", "x+x");
			Assert::IsTrue(actual == list<int>());

			actual = PKB::getPKBInstance()->getAssignWithBoth("xyz", "x+1");
			Assert::IsTrue(actual == list<int>());
		}
	};
}