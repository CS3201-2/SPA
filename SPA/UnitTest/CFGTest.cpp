#include "stdafx.h"
#include "CppUnitTest.h"
#include "CFG.h"
#include "SPALog.h"
const string EXCEPT_BOUND = "Exception: index out of bound!";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(CFGTest)
	{
		CFG constructInputNoNested()
		{
			//constructing CFG
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
			return cfg;
		}
	public:
		TEST_METHOD(testNextNoNestedFirst) 
		{
			CFG cfg = constructInputNoNested();
			list<int> result;
			result = { 1 };
			Assert::IsTrue(result == cfg.getNextFirst(2));
			result = { 5,9 };
			Assert::IsTrue(result == cfg.getNextFirst(6));
			result = { 6 };
			Assert::IsTrue(result == cfg.getNextFirst(10));
			result = { 10 };
			Assert::IsTrue(result == cfg.getNextFirst(11));
			result = {};
			Assert::IsTrue(result == cfg.getNextFirst(16));
			try
			{
				cfg.getNextFirst(18);
				Assert::Fail();
			}
			catch (exception e)
			{
				
			}
		}
		TEST_METHOD(testNextNoNestedSecond)
		{
			CFG cfg = constructInputNoNested();
			list<int> result;
			result = { 2 };
			Assert::IsTrue(result == cfg.getNextSecond(1));
			result = { 7,10 };
			Assert::IsTrue(result == cfg.getNextSecond(6));
			result = { 11, 12 };
			Assert::IsTrue(result == cfg.getNextSecond(10));
			result = { 13 };
			Assert::IsTrue(result == cfg.getNextSecond(11));
			result = {};
			Assert::IsTrue(result == cfg.getNextSecond(17));
			try
			{
				cfg.getNextFirst(18);
				Assert::Fail();
			}
			catch (exception e)
			{

			}
		}
		TEST_METHOD(testNextNoNestedValid)
		{
			CFG cfg = constructInputNoNested();
			bool result;
			result = true;
			Assert::IsTrue(result == cfg.isNextValid(1,2));
			result = true;
			Assert::IsTrue(result == cfg.isNextValid(6,10));
			result = false;
			Assert::IsTrue(result == cfg.isNextValid(11,12));
			result = true;
			Assert::IsTrue(result == cfg.isNextValid(11,13));
			result = false;
			Assert::IsTrue(result == cfg.isNextValid(15,16));
			try
			{
				cfg.isNextValid(18,19);
				Assert::Fail();
			}
			catch (exception e)
			{

			}
		}
		TEST_METHOD(testNextStarNoNestedFirst)
		{
			CFG cfg = constructInputNoNested();
			list<int> result;
			list<int> actual;
			result = { 2,1 };
			Assert::IsTrue(result == cfg.getNextStarFirst(3));
			result = { 4,5,6,7,8,9 };
			actual = cfg.getNextStarFirst(6);
			actual.sort();
			Assert::IsTrue(result == actual);
			result = { 4,5,6,7,8,9,10 };
			actual = cfg.getNextStarFirst(11);
			actual.sort();
			Assert::IsTrue(result == actual);
			try
			{
				cfg.getNextStarFirst(18);
				Assert::Fail();
			}
			catch (exception e)
			{

			}
		}
		TEST_METHOD(testNextStarNoNestedSecond)
		{
			CFG cfg = constructInputNoNested();
			list<int> result;
			list<int> actual;
			result = { 2,3 };
			Assert::IsTrue(result == cfg.getNextStarSecond(1));
			result = { 6,7,8,9,10,11,12,13,14,15 };
			actual = cfg.getNextStarSecond(6);
			actual.sort();
			Assert::IsTrue(result == actual);
			result = { 11,12,13,14,15 };
			actual = cfg.getNextStarSecond(10);
			actual.sort();
			Assert::IsTrue(result == actual);
			result = { 13,14,15 };
			actual = cfg.getNextStarSecond(11);
			actual.sort();
			Assert::IsTrue(result == actual);
			try
			{
				cfg.getNextStarSecond(18);
				Assert::Fail();
			}
			catch (exception e)
			{

			}
		}
		TEST_METHOD(testNextStarNoNestedValid)
		{
			CFG cfg = constructInputNoNested();
			bool result;
			result = true;
			Assert::IsTrue(result == cfg.isNextStarValid(1, 3));
			result = true;
			Assert::IsTrue(result == cfg.isNextStarValid(6, 6));
			result = false;
			Assert::IsTrue(result == cfg.isNextStarValid(11, 12));
			result = true;
			Assert::IsTrue(result == cfg.isNextStarValid(4, 15));
			result = false;
			Assert::IsTrue(result == cfg.isNextStarValid(1, 16));
			try
			{
				cfg.isNextStarValid(18, 19);
				Assert::Fail();
			}
			catch (exception e)
			{

			}
		}
		TEST_METHOD(testNextWithDummy)
		{
			CFG cfg = constructInputNoNested();
			cfg.printNextTableWithDummy();
			cfg.printDummyForNext();
			cfg.printBeforeTableWithDummy();
			cfg.printDummyForBefore();
		}
	};
}