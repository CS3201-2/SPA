#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "DesignExtractor.h"
#include "QueryEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(queryEvaluatorTest)
	{
	public:

		TEST_METHOD_INITIALIZE(buildPKB) {
				PKB::getPKBInstance()->addStmtToList(3, whileStmt);
				//assignList
				PKB::getPKBInstance()->addStmtToList(1, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(2, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(4, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(6, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(8, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(9, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(10, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(11, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(12, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(13, assignmentStmt);
				PKB::getPKBInstance()->addStmtToList(14, assignmentStmt);
				//callList
				PKB::getPKBInstance()->addStmtToList(5, procCallStmt);
				//ifList
				PKB::getPKBInstance()->addStmtToList(7, ifStmt);
				//constantList
				PKB::getPKBInstance()->addConstantToList(0);
				PKB::getPKBInstance()->addConstantToList(1);
				PKB::getPKBInstance()->addConstantToList(2);
				PKB::getPKBInstance()->addConstantToList(5);
				//call StmtMap
				PKB::getPKBInstance()->addToCallStmtProcMap(5, -2);
				//varTable
				PKB::getPKBInstance()->insertVar("x");
				PKB::getPKBInstance()->insertVar("i");
				PKB::getPKBInstance()->insertVar("y");
				PKB::getPKBInstance()->insertVar("z");
				PKB::getPKBInstance()->insertVar("v");
				//ProcTable
				PKB::getPKBInstance()->insertProc("Second");
				PKB::getPKBInstance()->insertProc("Third");
				//Modifies
				list<int> varList;
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
				varList.push_back(4);
				varList.push_back(5);
				PKB::getPKBInstance()->setModifies(3, varList);
				varList.clear();

				varList.push_back(1);
				PKB::getPKBInstance()->setModifies(4, varList);
				varList.clear();

				varList.push_back(4);
				varList.push_back(5);
				PKB::getPKBInstance()->setModifies(5, varList);
				varList.clear();

				varList.push_back(2);
				PKB::getPKBInstance()->setModifies(6, varList);
				varList.clear();

				varList.push_back(1);
				varList.push_back(4);
				PKB::getPKBInstance()->setModifies(7, varList);
				varList.clear();

				varList.push_back(1);
				PKB::getPKBInstance()->setModifies(8, varList);
				varList.clear();

				varList.push_back(4);
				PKB::getPKBInstance()->setModifies(9, varList);
				varList.clear();

				varList.push_back(4);
				PKB::getPKBInstance()->setModifies(10, varList);
				varList.clear();

				varList.push_back(3);
				PKB::getPKBInstance()->setModifies(11, varList);
				varList.clear();

				varList.push_back(1);
				PKB::getPKBInstance()->setModifies(12, varList);
				varList.clear();

				varList.push_back(4);
				PKB::getPKBInstance()->setModifies(13, varList);
				varList.clear();

				varList.push_back(5);
				PKB::getPKBInstance()->setModifies(14, varList);
				varList.clear();

				//Uses
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
				varList.push_back(3);
				PKB::getPKBInstance()->setUses(4, varList);
				varList.clear();

				varList.push_back(4);
				PKB::getPKBInstance()->setUses(5, varList);
				varList.clear();

				varList.push_back(2);
				PKB::getPKBInstance()->setUses(6, varList);
				varList.clear();

				varList.push_back(1);
				PKB::getPKBInstance()->setUses(7, varList);
				varList.clear();

				varList.push_back(1);
				PKB::getPKBInstance()->setUses(8, varList);
				varList.clear();

				varList.push_back(1);
				varList.push_back(2);
				varList.push_back(4);
				PKB::getPKBInstance()->setUses(10, varList);
				varList.clear();

				varList.push_back(4);
				PKB::getPKBInstance()->setUses(11, varList);
				varList.clear();

				varList.push_back(1);
				varList.push_back(3);
				varList.push_back(4);
				PKB::getPKBInstance()->setUses(12, varList);
				varList.clear();

				varList.push_back(4);
				PKB::getPKBInstance()->setUses(14, varList);
				varList.clear();

				//Follows
				PKB::getPKBInstance()->setFollows(1, 2);
				PKB::getPKBInstance()->setFollows(2, 3);
				PKB::getPKBInstance()->setFollows(3, 7);
				PKB::getPKBInstance()->setFollows(4, 5);
				PKB::getPKBInstance()->setFollows(5, 6);
				PKB::getPKBInstance()->setFollows(7, 10);
				PKB::getPKBInstance()->setFollows(10, 11);
				PKB::getPKBInstance()->setFollows(11, 12);
				PKB::getPKBInstance()->setFollows(13, 14);

				//Parent
				varList.push_back(4);
				varList.push_back(5);
				varList.push_back(6);
				PKB::getPKBInstance()->setParent(3, varList);
				varList.clear();

				varList.push_back(8);
				varList.push_back(9);
				PKB::getPKBInstance()->setParent(7, varList);
				varList.clear();

				//Pattern
				PKB::getPKBInstance()->setPattern(1, "x", "0");
				PKB::getPKBInstance()->setPattern(2, "i", "5");
				PKB::getPKBInstance()->setPattern(3, "i", "_while_");
				PKB::getPKBInstance()->setPattern(4, "x", "x+2*y");
				PKB::getPKBInstance()->setPattern(6, "i", "i-1");
				PKB::getPKBInstance()->setPattern(7, "x", "_if_");
				PKB::getPKBInstance()->setPattern(8, "x", "x+1");
				PKB::getPKBInstance()->setPattern(9, "z", "1");
				PKB::getPKBInstance()->setPattern(10, "z", "z+x+i");
				PKB::getPKBInstance()->setPattern(11, "y", "z+2");
				PKB::getPKBInstance()->setPattern(12, "x", "x*y+z");
				PKB::getPKBInstance()->setPattern(13, "z", "5");
				PKB::getPKBInstance()->setPattern(14, "v", "z");

				//call
				PKB::getPKBInstance()->setCalls(-1, -2);
				list<int> callStarList;
				callStarList.push_back(-2);
				PKB::getPKBInstance()->setCallsStar(-1, callStarList);

				DesignExtractor de = DesignExtractor();
				de.setFollowsStar();
				de.setParentStar();
				de.setReverseMap();

				//next and nextStar
				list<Statement> sourceList;
				Statement temp= Statement();
				temp.setNumber(-1); temp.setContent("procedureSecond{"); temp.setType(procDeclarationStmt); sourceList.push_back(temp);
				temp.setNumber(1); temp.setContent("x=0;"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(2); temp.setContent("i=5;"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(3); temp.setContent("whilei{"); temp.setType(whileStmt); sourceList.push_back(temp);
				temp.setNumber(4); temp.setContent("x=x+2*y;"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(5); temp.setContent("callThird;"); temp.setType(procCallStmt); sourceList.push_back(temp);
				temp.setNumber(6); temp.setContent("i=i-1;}"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(7); temp.setContent("ifxthen{"); temp.setType(ifStmt); sourceList.push_back(temp);
				temp.setNumber(8); temp.setContent("x=x+1;}"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(-1); temp.setContent("else{"); temp.setType(elseStmt); sourceList.push_back(temp);
				temp.setNumber(9); temp.setContent("z=1;}"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(10); temp.setContent("z=z+x+i;"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(11); temp.setContent("y=z+2;"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(12); temp.setContent("x=x*y+z;}"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(-1); temp.setContent("procedureThird{"); temp.setType(procDeclarationStmt); sourceList.push_back(temp);
				temp.setNumber(13); temp.setContent("z=5;"); temp.setType(assignmentStmt); sourceList.push_back(temp);
				temp.setNumber(14); temp.setContent("v=z;}"); temp.setType(assignmentStmt); sourceList.push_back(temp);

				PKB::getPKBInstance()->buildCFG(sourceList);
			
		}

		TEST_METHOD_CLEANUP(clearup) {
			SPALog::log("One evaluate test complete!\n\n");
			PKB::DestroyInstance();
		}

		TEST_METHOD(evaluateModifies)
		{
			/*
			 test includes:
			 Select s such that Modifies(s, "x") Expected: 1, 3, 4, 7, 8, 12
			 Select v such that Modifies("Second", v) Expected: x, i, y, z, v
			 Select v such that Modifies("Foobar", v) Expected:
			 Select v such that Modifies(5, v) Expected: z, v
			 Select p such that Modifies(p, _) Expected: Second, Third
			 Select p such that Modifies(p, "m") Expected:
			 Select p such that Modifies(4, "i") Expected :
			 Select v such that Modifies(7, v) Expected: x, z
			 Select BOOLEAN such that Modifies("Second", "x") Expected: true
			 Select BOOLEAN such that Modifies(2, "x") Expected: false
			*/
		
			//Select s such that Modifies(s, "x") Expected: 1, 3, 4, 7, 8, 12
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("s");
			arg.push_back("x");
			argType.push_back("stmt");
			argType.push_back("string");
			qt.insertSuchThat("modifies", arg, argType);

			qt.insertSelect("s", "stmt");

			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("12");
			expected.push_back("3");
			expected.push_back("4");
			expected.push_back("7");
			expected.push_back("8");
			Assert::IsTrue(expected == result);

			//Select v such that Modifies("Second", v) Expected: x, i, y, z, v
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("Second");
			arg_2.push_back("v");
			argType_2.push_back("string");
			argType_2.push_back("variable");
			qt_2.insertSuchThat("modifies", arg_2, argType_2);

			qt_2.insertSelect("v", "variable");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("i");
			expected_2.push_back("v");
			expected_2.push_back("x");
			expected_2.push_back("y");
			expected_2.push_back("z");
			Assert::IsTrue(expected_2 == result_2);


			//Select v such that Modifies("Foobar", v) Expected: 
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("Foobar");
			arg_3.push_back("v");
			argType_3.push_back("string");
			argType_3.push_back("variable");
			qt_3.insertSuchThat("modifies", arg_3, argType_3);

			qt_3.insertSelect("v", "variable");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			Assert::IsTrue(expected_3 == result_3);


			//Select v such that Modifies(5, v) Expected: z, v
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("5");
			arg_4.push_back("v");
			argType_4.push_back("number");
			argType_4.push_back("variable");
			qt_4.insertSuchThat("modifies", arg_4, argType_4);

			qt_4.insertSelect("v", "variable");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("v");
			expected_4.push_back("z");
			Assert::IsTrue(expected_4 == result_4);


			//Select p such that Modifies(p, _) Expected: Second, Third
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("p");
			arg_5.push_back("_");
			argType_5.push_back("procedure");
			argType_5.push_back("all");
			qt_5.insertSuchThat("modifies", arg_5, argType_5);

			qt_5.insertSelect("p", "procedure");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("Second");
			expected_5.push_back("Third");
			Assert::IsTrue(expected_5 == result_5);

			//Select p such that Modifies(p, "m") Expected:
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("p");
			arg_6.push_back("m");
			argType_6.push_back("procedure");
			argType_6.push_back("string");
			qt_6.insertSuchThat("modifies", arg_6, argType_6);

			qt_6.insertSelect("p", "procedure");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			Assert::IsTrue(expected_6 == result_6);

			//Select p such that Modifies(4, "i") Expected:
			QueryTree qt_7;
			vector<string> arg_7;
			vector<string> argType_7;
			arg_7.push_back("4");
			arg_7.push_back("i");
			argType_7.push_back("number");
			argType_7.push_back("string");
			qt_7.insertSuchThat("modifies", arg_7, argType_7);
			qt_7.insertSelect("p", "procedure");
			QueryEvaluator qe_7 = (qt_7);
			list<string> result_7 = qe_7.evaluate();
			list<string> expected_7;
			Assert::IsTrue(expected_7 == result_7);
			
			//Select v such that Modifies(7, v) Expected: x, z
			QueryTree qt_8;
			vector<string> arg_8;
			vector<string> argType_8;
			arg_8.push_back("7");
			arg_8.push_back("v");
			argType_8.push_back("number");
			argType_8.push_back("variable");
			qt_8.insertSuchThat("modifies", arg_8, argType_8);
			qt_8.insertSelect("v", "variable");
			QueryEvaluator qe_8 = (qt_8);
			list<string> result_8 = qe_8.evaluate();
			list<string> expected_8;
			expected_8.push_back("x");
			expected_8.push_back("z");
			Assert::IsTrue(expected_8 == result_8);

			//Select BOOLEAN such that Modifies("Second", "x") Expected: true
			QueryTree qt_9;
			vector<string> arg_9;
			vector<string> argType_9;
			arg_9.push_back("Second");
			arg_9.push_back("x");
			argType_9.push_back("string");
			argType_9.push_back("string");
			qt_9.insertSuchThat("modifies", arg_9, argType_9);
			qt_9.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_9 = (qt_9);
			list<string> result_9 = qe_9.evaluate();
			list<string> expected_9;
			expected_9.push_back("TRUE");
			Assert::IsTrue(expected_9 == result_9);

			//Select BOOLEAN such that Modifies(2, "x") Expected: false
			QueryTree qt_10;
			vector<string> arg_10;
			vector<string> argType_10;
			arg_10.push_back("2");
			arg_10.push_back("x");
			argType_10.push_back("string");
			argType_10.push_back("string");
			qt_10.insertSuchThat("modifies", arg_10, argType_10);
			qt_10.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_10 = (qt_10);
			list<string> result_10 = qe_10.evaluate();
			list<string> expected_10;
			expected_10.push_back("FALSE");
			Assert::IsTrue(expected_10 == result_10);
		}

		TEST_METHOD(evaluateUses)
		{
			/*
			test includes:
			Select s such that Uses(s, "x") Expected: 3, 4, 7, 8, 10, 12
			Select v such that Uses("Second", v) Expected: x, i, y, z
			Select v such that Uses("Foobar", v) Expected:
			Select v such that Uses(5, v) Expected: z
			Select p such that Uses(p, _) Expected: Second, Third
			Select p such that Uses(p, "m") Expected:
			Select p such that Uses(4, "i") Expected :
			Select v such that Uses(7, v) Expected: x
			Select BOOLEAN such that uses(2, "x") Expected: false
			Select BOOLEAN such that Use("Second", "x") Expected: true
			*/

			//Select s such that Uses(s, "x") Expected: 3, 4, 7, 8, 10, 12
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("s");
			arg.push_back("x");
			argType.push_back("stmt");
			argType.push_back("string");
			qt.insertSuchThat("uses", arg, argType);

			qt.insertSelect("s", "stmt");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
		
			expected.push_back("10");
			expected.push_back("12");
			expected.push_back("3");
			expected.push_back("4");
			expected.push_back("7");
			expected.push_back("8");
			Assert::IsTrue(expected == result );

			//Select v such that Uses("Second", v) Expected: x, i, y, z
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("Second");
			arg_2.push_back("v");
			argType_2.push_back("string");
			argType_2.push_back("variable");
			qt_2.insertSuchThat("uses", arg_2, argType_2);

			qt_2.insertSelect("v", "variable");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("i");
			expected_2.push_back("x");
			expected_2.push_back("y");
			expected_2.push_back("z");
			Assert::IsTrue(expected_2 == result_2);


			//Select v such that Modifies("Foobar", v) Expected: 
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("Foobar");
			arg_3.push_back("v");
			argType_3.push_back("string");
			argType_3.push_back("variable");
			qt_3.insertSuchThat("uses", arg_3, argType_3);

			qt_3.insertSelect("v", "variable");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			Assert::IsTrue(expected_3 == result_3);


			//Select v such that Uses(5, v) Expected: z
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("5");
			arg_4.push_back("v");
			argType_4.push_back("number");
			argType_4.push_back("variable");
			qt_4.insertSuchThat("uses", arg_4, argType_4);

			qt_4.insertSelect("v", "variable");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("z");
			Assert::IsTrue(expected_4 == result_4);


			//Select p such that Uses(p, _) Expected: Second, Third
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("p");
			arg_5.push_back("_");
			argType_5.push_back("procedure");
			argType_5.push_back("all");
			qt_5.insertSuchThat("uses", arg_5, argType_5);

			qt_5.insertSelect("p", "procedure");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("Second");
			expected_5.push_back("Third");
			Assert::IsTrue(expected_5 == result_5);

			//Select p such that Uses(p, "m") Expected:
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("p");
			arg_6.push_back("m");
			argType_6.push_back("procedure");
			argType_6.push_back("string");
			qt_6.insertSuchThat("uses", arg_6, argType_6);

			qt_6.insertSelect("p", "procedure");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			Assert::IsTrue(expected_6 == result_6);

			//Select p such that Uses(4, "i") Expected:
			QueryTree qt_7;
			vector<string> arg_7;
			vector<string> argType_7;
			arg_7.push_back("4");
			arg_7.push_back("i");
			argType_7.push_back("number");
			argType_7.push_back("string");
			qt_7.insertSuchThat("uses", arg_7, argType_7);
			qt_7.insertSelect("p", "procedure");
			QueryEvaluator qe_7 = (qt_7);
			list<string> result_7 = qe_7.evaluate();
			list<string> expected_7;
			Assert::IsTrue(expected_7 == result_7);

			//Select v such that Uses(7, v) Expected: x
			QueryTree qt_8;
			vector<string> arg_8;
			vector<string> argType_8;
			arg_8.push_back("7");
			arg_8.push_back("v");
			argType_8.push_back("number");
			argType_8.push_back("variable");
			qt_8.insertSuchThat("uses", arg_8, argType_8);
			qt_8.insertSelect("v", "variable");
			QueryEvaluator qe_8 = (qt_8);
			list<string> result_8 = qe_8.evaluate();
			list<string> expected_8;
			expected_8.push_back("x");
			Assert::IsTrue(expected_8 == result_8);

			//Select BOOLEAN such that Use("Second", "x") Expected: true
			QueryTree qt_9;
			vector<string> arg_9;
			vector<string> argType_9;
			arg_9.push_back("Second");
			arg_9.push_back("x");
			argType_9.push_back("string");
			argType_9.push_back("string");
			qt_9.insertSuchThat("uses", arg_9, argType_9);
			qt_9.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_9 = (qt_9);
			list<string> result_9 = qe_9.evaluate();
			list<string> expected_9;
			expected_9.push_back("TRUE");
			Assert::IsTrue(expected_9 == result_9);

			//Select BOOLEAN such that uses(2, "x") Expected: false
			QueryTree qt_10;
			vector<string> arg_10;
			vector<string> argType_10;
			arg_10.push_back("2");
			arg_10.push_back("x");
			argType_10.push_back("string");
			argType_10.push_back("string");
			qt_10.insertSuchThat("uses", arg_10, argType_10);
			qt_10.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_10 = (qt_10);
			list<string> result_10 = qe_10.evaluate();
			list<string> expected_10;
			expected_10.push_back("FALSE");
			Assert::IsTrue(expected_10 == result_10);
		}
		
		TEST_METHOD(evaluateParent) {
			/*
			Select s such that Parent(7, s) Expected: 8,9
			Select s such that Parent(5, s) Expected:
			Select w such that Parent(w, _) Expected: 3
			Select ifs such that Parent( ifs , 8) Expected:7
			Select a such that Parent(w, a) Expected: 4,6
			Select BOOLEAN such that Parent(3, 4) Expected: true
			Select BOOLEAN such that Parent(3, 2) Expected: false
			*/

			//Select s such that Parent(7, s) Expected: 8,9
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("7");
			arg.push_back("s");
			argType.push_back("number");
			argType.push_back("stmt");
			qt.insertSuchThat("parent", arg, argType);

			qt.insertSelect("s", "stmt");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("8");
			expected.push_back("9");
			Assert::IsTrue(expected == result);

			//Select s such that Parent(5, s) Expected:
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("5");
			arg_1.push_back("s");
			argType_1.push_back("number");
			argType_1.push_back("stmt");
			qt_1.insertSuchThat("parent", arg_1, argType_1);

			qt_1.insertSelect("s", "stmt");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			Assert::IsTrue(expected_1 == result_1);

			//Select w such that Parent(w, _) Expected: 3
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("w");
			arg_2.push_back("_");
			argType_2.push_back("while");
			argType_2.push_back("all");
			qt_2.insertSuchThat("parent", arg_2, argType_2);

			qt_2.insertSelect("w", "while");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("3");
			Assert::IsTrue(expected_2 == result_2);

			//Select ifs such that Parent( ifs , 8) Expected:7
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("ifs");
			arg_3.push_back("8");
			argType_3.push_back("if");
			argType_3.push_back("number");
			qt_3.insertSuchThat("parent", arg_3, argType_3);

			qt_3.insertSelect("ifs", "if");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("7");
			Assert::IsTrue(expected_3 == result_3);
		
			//Select a such that Parent(w, a) Expected: 4, 6
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("w");
			arg_4.push_back("a");
			argType_4.push_back("while");
			argType_4.push_back("assign");
			qt_4.insertSuchThat("parent", arg_4, argType_4);

			qt_4.insertSelect("a", "assign");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("4");
			expected_4.push_back("6");
			Assert::IsTrue(expected_4 == result_4);

			//Select BOOLEAN such that Parent(3, 4) Expected: true
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("3");
			arg_5.push_back("4");
			argType_5.push_back("number");
			argType_5.push_back("number");
			qt_5.insertSuchThat("parent", arg_5, argType_5);

			qt_5.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("TRUE");
			Assert::IsTrue(expected_5 == result_5);

			//Select BOOLEAN such that Parent(3, 4) Expected: true
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("3");
			arg_6.push_back("2");
			argType_6.push_back("number");
			argType_6.push_back("number");
			qt_6.insertSuchThat("parent", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("FALSE");
			Assert::IsTrue(expected_6 == result_6);
		}

		TEST_METHOD(evaluateParentStar) {
			/*
			Select s such that Parent*(7, s) Expected: 8,9
			Select s such that Parent*(5, s) Expected:
			Select w such that Parent*(w, _) Expected: 3
			Select ifs such that Parent*( ifs , 8) Expected:7
			Select a such that Parent*(w, a) Expected: 4,6
			Select BOOLEAN such that Parent*(3, 4) Expected: true
			Select BOOLEAN such that Parent*(3, 2) Expected: false
			*/

			//Select s such that Parent*(7, s) Expected: 8,9
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("7");
			arg.push_back("s");
			argType.push_back("number");
			argType.push_back("stmt");
			qt.insertSuchThat("parent*", arg, argType);

			qt.insertSelect("s", "stmt");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("8");
			expected.push_back("9");
			Assert::IsTrue(expected == result);

			//Select s such that Parent*(5, s) Expected:
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("5");
			arg_1.push_back("s");
			argType_1.push_back("number");
			argType_1.push_back("stmt");
			qt_1.insertSuchThat("parent*", arg_1, argType_1);

			qt_1.insertSelect("s", "stmt");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			Assert::IsTrue(expected_1 == result_1);

			//Select w such that Parent*(w, _) Expected: 3
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("w");
			arg_2.push_back("_");
			argType_2.push_back("while");
			argType_2.push_back("all");
			qt_2.insertSuchThat("parent*", arg_2, argType_2);

			qt_2.insertSelect("w", "while");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("3");
			Assert::IsTrue(expected_2 == result_2);

			//Select ifs such that Parent*( ifs , 8) Expected:7
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("ifs");
			arg_3.push_back("8");
			argType_3.push_back("if");
			argType_3.push_back("number");
			qt_3.insertSuchThat("parent*", arg_3, argType_3);

			qt_3.insertSelect("ifs", "if");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("7");
			Assert::IsTrue(expected_3 == result_3);

			//Select a such that Parent*(w, a) Expected: 4, 6
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("w");
			arg_4.push_back("a");
			argType_4.push_back("while");
			argType_4.push_back("assign");
			qt_4.insertSuchThat("parent*", arg_4, argType_4);

			qt_4.insertSelect("a", "assign");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("4");
			expected_4.push_back("6");
			Assert::IsTrue(expected_4 == result_4);

			//Select BOOLEAN such that Parent*(3, 4) Expected: true
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("3");
			arg_5.push_back("4");
			argType_5.push_back("number");
			argType_5.push_back("number");
			qt_5.insertSuchThat("parent*", arg_5, argType_5);

			qt_5.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("TRUE");
			Assert::IsTrue(expected_5 == result_5);

			//Select BOOLEAN such that Parent*(3, 4) Expected: true
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("3");
			arg_6.push_back("2");
			argType_6.push_back("number");
			argType_6.push_back("number");
			qt_6.insertSuchThat("parent*", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("FALSE");
			Assert::IsTrue(expected_6 == result_6);
		}

		TEST_METHOD(evaluateFollows) {
			/*
			Select s such that Follows(1, s) Expected: 2
			Select s such that Follows(6, s) Expected:
			Select w such that Follows(w, _) Expected: 3
			Select ifs such that Follows( ifs , 10) Expected:7
			Select s such that Follows(w, s) Expected:7 
			Select BOOLEAN such that Follows(2, 3) Expected: true
			Select BOOLEAN such that Follows(3, 4) Expected: false
			*/

			//Select s such that Follows(1, s) Expected: 2
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("1");
			arg.push_back("s");
			argType.push_back("number");
			argType.push_back("stmt");
			qt.insertSuchThat("follows", arg, argType);

			qt.insertSelect("s", "stmt");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("2");
			Assert::IsTrue(expected == result);

			//Select s such that Follows(6, s) Expected:
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("6");
			arg_1.push_back("s");
			argType_1.push_back("number");
			argType_1.push_back("stmt");
			qt_1.insertSuchThat("follows", arg_1, argType_1);

			qt_1.insertSelect("s", "stmt");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			Assert::IsTrue(expected_1 == result_1);

			//Select w such that Follows(w, _) Expected: 3
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("w");
			arg_2.push_back("_");
			argType_2.push_back("while");
			argType_2.push_back("all");
			qt_2.insertSuchThat("follows", arg_2, argType_2);

			qt_2.insertSelect("w", "while");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("3");
			Assert::IsTrue(expected_2 == result_2);

			//Select ifs such that Follows( ifs , 10) Expected:7
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("ifs");
			arg_3.push_back("10");
			argType_3.push_back("if");
			argType_3.push_back("number");
			qt_3.insertSuchThat("follows", arg_3, argType_3);

			qt_3.insertSelect("ifs", "if");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("7");
			Assert::IsTrue(expected_3 == result_3);

			//Select s such that Follows(w, s) Expected: 7
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("w");
			arg_4.push_back("s");
			argType_4.push_back("while");
			argType_4.push_back("stmt");
			qt_4.insertSuchThat("follows", arg_4, argType_4);

			qt_4.insertSelect("s", "stmt");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("7");
			Assert::IsTrue(expected_4 == result_4);

			//Select BOOLEAN such that Follows(2, 3) Expected: true
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("2");
			arg_5.push_back("3");
			argType_5.push_back("number");
			argType_5.push_back("number");
			qt_5.insertSuchThat("follows", arg_5, argType_5);

			qt_5.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("TRUE");
			Assert::IsTrue(expected_5 == result_5);

			//Select BOOLEAN such that Follows(3, 4) Expected: true
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("3");
			arg_6.push_back("2");
			argType_6.push_back("number");
			argType_6.push_back("number");
			qt_6.insertSuchThat("follows", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("FALSE");
			Assert::IsTrue(expected_6 == result_6);
		}

		TEST_METHOD(evaluateFollowsStar) {
			/*
			Select s such that Follows*(4, s) Expected: 5,6
			Select s such that Follows*(6, s) Expected:
			Select w such that Follows*(w, _) Expected: 3
			Select ifs such that Follows*( ifs , 11) Expected:7
			Select s such that Follows*(w, s) Expected:7,10,11,12
			Select BOOLEAN such that Follows*(2, 3) Expected: true
			Select BOOLEAN such that Follows*(3, 4) Expected: false
			*/

			//Select s such that Follows*(4, s) Expected: 5,6
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("4");
			arg.push_back("s");
			argType.push_back("number");
			argType.push_back("stmt");
			qt.insertSuchThat("follows*", arg, argType);

			qt.insertSelect("s", "stmt");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("5");
			expected.push_back("6");
			Assert::IsTrue(expected == result);

			//Select s such that Follows*(6, s) Expected:
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("6");
			arg_1.push_back("s");
			argType_1.push_back("number");
			argType_1.push_back("stmt");
			qt_1.insertSuchThat("follows*", arg_1, argType_1);

			qt_1.insertSelect("s", "stmt");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			Assert::IsTrue(expected_1 == result_1);

			//Select w such that Follows*(w, _) Expected: 3
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("w");
			arg_2.push_back("_");
			argType_2.push_back("while");
			argType_2.push_back("all");
			qt_2.insertSuchThat("follows*", arg_2, argType_2);

			qt_2.insertSelect("w", "while");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("3");
			Assert::IsTrue(expected_2 == result_2);

			//Select ifs such that Follows*( ifs , 11) Expected:7
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("ifs");
			arg_3.push_back("11");
			argType_3.push_back("if");
			argType_3.push_back("number");
			qt_3.insertSuchThat("follows*", arg_3, argType_3);

			qt_3.insertSelect("ifs", "if");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("7");
			Assert::IsTrue(expected_3 == result_3);

			//Select s such that Follows*(w, s) Expected: 7,10,11,12
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("w");
			arg_4.push_back("s");
			argType_4.push_back("while");
			argType_4.push_back("stmt");
			qt_4.insertSuchThat("follows*", arg_4, argType_4);

			qt_4.insertSelect("s", "stmt");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("10");
			expected_4.push_back("11");
			expected_4.push_back("12");
			expected_4.push_back("7");
			Assert::IsTrue(expected_4 == result_4);

			//Select BOOLEAN such that Follows*(2, 3) Expected: true
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("2");
			arg_5.push_back("3");
			argType_5.push_back("number");
			argType_5.push_back("number");
			qt_5.insertSuchThat("follows*", arg_5, argType_5);

			qt_5.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("TRUE");
			Assert::IsTrue(expected_5 == result_5);

			//Select BOOLEAN such that Follows*(3, 4) Expected: true
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("3");
			arg_6.push_back("2");
			argType_6.push_back("number");
			argType_6.push_back("number");
			qt_6.insertSuchThat("follows*", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("FALSE");
			Assert::IsTrue(expected_6 == result_6);
		}

		TEST_METHOD(evaluateNext) {
			/*
			Select s such that Next(3, s) Expected: 4,7
			Select s such that Next(100, s) Expected:
			Select w such that Next(w, _) Expected: 3
			Select s such that Next( c , s) Expected:6
			Select ifs such that Next(_, ifs) Expected:7
			Select BOOLEAN such that next(3,7) Expected: true
			Select BOOLEAN such that next(3,5) Expected: false
			Select c such that next(c, 6) Expected: 5
			*/

			//Select s such that Next(3, s) Expected: 4,7
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("3");
			arg.push_back("s");
			argType.push_back("number");
			argType.push_back("stmt");
			qt.insertSuchThat("next", arg, argType);

			qt.insertSelect("s", "stmt");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("4");
			expected.push_back("7");
			Assert::IsTrue(expected == result);

			//Select s such that Next(100, s) Expected:
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("100");
			arg_1.push_back("s");
			argType_1.push_back("number");
			argType_1.push_back("stmt");
			qt_1.insertSuchThat("next", arg_1, argType_1);

			qt_1.insertSelect("s", "stmt");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			Assert::IsTrue(expected_1 == result_1);

			//Select w such that Next(w, _) Expected: 3
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("w");
			arg_2.push_back("_");
			argType_2.push_back("while");
			argType_2.push_back("all");
			qt_2.insertSuchThat("next", arg_2, argType_2);

			qt_2.insertSelect("w", "while");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("3");
			Assert::IsTrue(expected_2 == result_2);

			//Select s such that Next( c , s) Expected:6
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("c");
			arg_3.push_back("s");
			argType_3.push_back("call");
			argType_3.push_back("stmt");
			qt_3.insertSuchThat("next", arg_3, argType_3);

			qt_3.insertSelect("s", "stmt");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("6");
			Assert::IsTrue(expected_3 == result_3);

			//Select ifs such that Next(_, ifs) Expected:7
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("_");
			arg_4.push_back("ifs");
			argType_4.push_back("all");
			argType_4.push_back("if");
			qt_4.insertSuchThat("next", arg_4, argType_4);

			qt_4.insertSelect("ifs", "if");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("7");
			Assert::IsTrue(expected_4 == result_4);

			//Select BOOLEAN such that next(3,7) Expected: true
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("3");
			arg_5.push_back("7");
			argType_5.push_back("number");
			argType_5.push_back("number");
			qt_5.insertSuchThat("next", arg_5, argType_5);

			qt_5.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("TRUE");
			Assert::IsTrue(expected_5 == result_5);

			//Select BOOLEAN such that next(3,5) Expected: false
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("3");
			arg_6.push_back("5");
			argType_6.push_back("number");
			argType_6.push_back("number");
			qt_6.insertSuchThat("next", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("FALSE");
			Assert::IsTrue(expected_6 == result_6);
		
			//Select c such that next(c, 6) Expected: 5
			QueryTree qt_7;
			vector<string> arg_7;
			vector<string> argType_7;
			arg_7.push_back("c");
			arg_7.push_back("6");
			argType_7.push_back("call");
			argType_7.push_back("number");
			qt_7.insertSuchThat("next", arg_7, argType_7);

			qt_7.insertSelect("c", "call");
			QueryEvaluator qe_7 = (qt_7);
			list<string> result_7 = qe_7.evaluate();
			list<string> expected_7;
			expected_7.push_back("5");
			Assert::IsTrue(expected_7 == result_7);
		}

		TEST_METHOD(evaluateNextStar) {
			/*
			Select s such that Next*(9, s) Expected: 10,11,12
			Select s such that Next*(100, s) Expected:
			Select w such that Next*(w, _) Expected: 3
			Select a such that Next*( if , a) Expected:8,9,10,11,12
			Select ifs such that Next*(_, ifs) Expected:7
			Select BOOLEAN such that next*(3,7) Expected: true
			Select BOOLEAN such that next*(8,9) Expected: false
			Select c such that next*(c, 6) Expected: 5
			*/

			//Select s such that Next*(9, s) Expected: 10,11,12
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("9");
			arg.push_back("s");
			argType.push_back("number");
			argType.push_back("stmt");
			qt.insertSuchThat("next*", arg, argType);

			qt.insertSelect("s", "stmt");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			Assert::IsTrue(expected == result);

			//Select s such that Next*(100, s) Expected:
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("100");
			arg_1.push_back("s");
			argType_1.push_back("number");
			argType_1.push_back("stmt");
			qt_1.insertSuchThat("next*", arg_1, argType_1);

			qt_1.insertSelect("s", "stmt");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			Assert::IsTrue(expected_1 == result_1);

			//Select w such that Next*(w, _) Expected: 3
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("w");
			arg_2.push_back("_");
			argType_2.push_back("while");
			argType_2.push_back("all");
			qt_2.insertSuchThat("next*", arg_2, argType_2);

			qt_2.insertSelect("w", "while");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("3");
			Assert::IsTrue(expected_2 == result_2);

			//Select a such that Next*( ifs , a) Expected:8,9,10,11,12
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("ifs");
			arg_3.push_back("a");
			argType_3.push_back("if");
			argType_3.push_back("assign");
			qt_3.insertSuchThat("next*", arg_3, argType_3);

			qt_3.insertSelect("a", "assign");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			
			expected_3.push_back("10");
			expected_3.push_back("11");
			expected_3.push_back("12");
			expected_3.push_back("8");
			expected_3.push_back("9");
			Assert::IsTrue(expected_3 == result_3);

			//Select ifs such that Next*(_, ifs) Expected:7
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("_");
			arg_4.push_back("ifs");
			argType_4.push_back("all");
			argType_4.push_back("if");
			qt_4.insertSuchThat("next*", arg_4, argType_4);

			qt_4.insertSelect("ifs", "if");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("7");
			Assert::IsTrue(expected_4 == result_4);

			//Select BOOLEAN such that next*(3,7) Expected: true
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("3");
			arg_5.push_back("7");
			argType_5.push_back("number");
			argType_5.push_back("number");
			qt_5.insertSuchThat("next*", arg_5, argType_5);

			qt_5.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("TRUE");
			Assert::IsTrue(expected_5 == result_5);

			//Select BOOLEAN such that next*(8,9) Expected: false
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("8");
			arg_6.push_back("9");
			argType_6.push_back("number");
			argType_6.push_back("number");
			qt_6.insertSuchThat("next*", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("FALSE");
			Assert::IsTrue(expected_6 == result_6);

			//Select c such that next*(c, 6) Expected: 5
			QueryTree qt_7;
			vector<string> arg_7;
			vector<string> argType_7;
			arg_7.push_back("c");
			arg_7.push_back("6");
			argType_7.push_back("call");
			argType_7.push_back("number");
			qt_7.insertSuchThat("next*", arg_7, argType_7);

			qt_7.insertSelect("c", "call");
			QueryEvaluator qe_7 = (qt_7);
			list<string> result_7 = qe_7.evaluate();
			list<string> expected_7;
			expected_7.push_back("5");
			Assert::IsTrue(expected_7 == result_7);
		}
		
		TEST_METHOD(evaluateCalls) {
			/*
			Select p such that Calls("Second", p) Expected: Third
			Select p1 such that Calls(p1, p2) Expected: Second
			Select p such that Calls(_, "Third") Expected: Second Third
			Select p such that Calls("First", _) Expected:
			Select p such that Calls(_, "Second") Expected:
			Select p such that Calls(p, p) Expected:
			Select BOOLEAN such that Calls("Second", "Third") Expected: true
			*/

			//Select p such that Calls("Second", p) Expected: Third
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("Second");
			arg.push_back("p");
			argType.push_back("string");
			argType.push_back("procedure");
			qt.insertSuchThat("calls", arg, argType);

			qt.insertSelect("p", "procedure");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("Third");
			Assert::IsTrue(expected == result);

			//Select p1 such that Calls(p1, p2) Expected: Second
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("p1");
			arg_1.push_back("p2");
			argType_1.push_back("procedure");
			argType_1.push_back("procedure");
			qt_1.insertSuchThat("calls", arg_1, argType_1);

			qt_1.insertSelect("p1", "procedure");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("Second");
			Assert::IsTrue(expected_1 == result_1);

			//Select p such that Calls(_, "Third") Expected: Second, Third
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("_");
			arg_2.push_back("Third");
			argType_2.push_back("all");
			argType_2.push_back("string");
			qt_2.insertSuchThat("calls", arg_2, argType_2);

			qt_2.insertSelect("p", "procedure");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("Second");
			expected_2.push_back("Third");
			Assert::IsTrue(expected_2 == result_2);

			//Select p such that Calls("First", _) Expected:
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("First");
			arg_3.push_back("_");
			argType_3.push_back("string");
			argType_3.push_back("all");
			qt_3.insertSuchThat("calls", arg_3, argType_3);

			qt_3.insertSelect("p", "procedure");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			Assert::IsTrue(expected_3 == result_3);

			//Select p such that Calls(_, "Second") Expected:
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("_");
			arg_4.push_back("Second");
			argType_4.push_back("all");
			argType_4.push_back("string");
			qt_4.insertSuchThat("calls", arg_4, argType_4);

			qt_4.insertSelect("p", "procedure");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			Assert::IsTrue(expected_4 == result_4);

			//Select p such that Calls(p, p) Expected:
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("p");
			arg_5.push_back("p");
			argType_5.push_back("procedure");
			argType_5.push_back("procedure");
			qt_5.insertSuchThat("calls", arg_5, argType_5);

			qt_5.insertSelect("p", "procedure");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			Assert::IsTrue(expected_5 == result_5);

			//Select p such that Calls("Second", "Third") Expected: true
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("Second");
			arg_6.push_back("Third");
			argType_6.push_back("string");
			argType_6.push_back("string");
			qt_6.insertSuchThat("calls", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("TRUE");
			Assert::IsTrue(expected_6 == result_6);
		}

		TEST_METHOD(evaluateCallsStar) {
			/*
			Select p such that Calls*("Second", p) Expected: Third
			Select p1 such that Calls*(p1, p2) Expected: Second
			Select p such that Calls*(_, "Third") Expected: Second Third
			Select p such that Calls*("First", _) Expected:
			Select p such that Calls*(_, "Second") Expected:
			Select p such that Calls*(p, p) Expected:
			Select BOOLEAN such that Calls*("Second", "Third") Expected: true
			*/

			//Select p such that Calls*("Second", p) Expected: Third
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("Second");
			arg.push_back("p");
			argType.push_back("string");
			argType.push_back("procedure");
			qt.insertSuchThat("calls*", arg, argType);

			qt.insertSelect("p", "procedure");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("Third");
			Assert::IsTrue(expected == result);

			//Select p1 such that Calls*(p1, p2) Expected: Second
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("p1");
			arg_1.push_back("p2");
			argType_1.push_back("procedure");
			argType_1.push_back("procedure");
			qt_1.insertSuchThat("calls*", arg_1, argType_1);

			qt_1.insertSelect("p1", "procedure");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("Second");
			Assert::IsTrue(expected_1 == result_1);

			//Select p such that Calls*(_, "Third") Expected: Second, Third
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("_");
			arg_2.push_back("Third");
			argType_2.push_back("all");
			argType_2.push_back("string");
			qt_2.insertSuchThat("calls*", arg_2, argType_2);

			qt_2.insertSelect("p", "procedure");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("Second");
			expected_2.push_back("Third");
			Assert::IsTrue(expected_2 == result_2);

			//Select p such that Calls*("First", _) Expected:
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("First");
			arg_3.push_back("_");
			argType_3.push_back("string");
			argType_3.push_back("all");
			qt_3.insertSuchThat("calls*", arg_3, argType_3);

			qt_3.insertSelect("p", "procedure");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			Assert::IsTrue(expected_3 == result_3);

			//Select p such that Calls*(_, "Second") Expected:
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("_");
			arg_4.push_back("Second");
			argType_4.push_back("all");
			argType_4.push_back("string");
			qt_4.insertSuchThat("calls*", arg_4, argType_4);

			qt_4.insertSelect("p", "procedure");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			Assert::IsTrue(expected_4 == result_4);

			//Select p such that Calls*(p, p) Expected:
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("p");
			arg_5.push_back("p");
			argType_5.push_back("procedure");
			argType_5.push_back("procedure");
			qt_5.insertSuchThat("calls*", arg_5, argType_5);

			qt_5.insertSelect("p", "procedure");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			Assert::IsTrue(expected_5 == result_5);

			//Select p such that Calls*("Second", "Third") Expected: true
			QueryTree qt_6;
			vector<string> arg_6;
			vector<string> argType_6;
			arg_6.push_back("Second");
			arg_6.push_back("Third");
			argType_6.push_back("string");
			argType_6.push_back("string");
			qt_6.insertSuchThat("calls*", arg_6, argType_6);

			qt_6.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_6 = (qt_6);
			list<string> result_6 = qe_6.evaluate();
			list<string> expected_6;
			expected_6.push_back("TRUE");
			Assert::IsTrue(expected_6 == result_6);
		}

		TEST_METHOD(evaluateAssignPattern) {
			/*
			Select a pattern a("x","0") Expected: 1
			Select a pattern a(_,_"x*y"_) Expected: 12
			Select BOOLEAN pattern a(_,_"y+z"_) Expected: false
			Select BOOLEAN pattern a("x",_) Expected: true
			Select BOOLEAN pattern a("x", "x*y+z") Expected: true
			Select BOOLEAN pattern a("x", _"x*y"_) Expected: true
			Select v pattern a(v,_) Expected: x,i,y,z,v
			Select v pattern a(v,x*y+1) Expected: x
			Select v pattern a(v,_"x*y"_) Expected: x
			Select BOOLEAN pattern a(_,_) Expected: true
			*/
			//Select a pattern a("x","0") Expected: 1
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("x");
			arg.push_back("0");
			argType.push_back("string");
			argType.push_back("string");
			qt.insertPattern("a","assign", arg, argType);

			qt.insertSelect("a", "assign");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("1");
			Assert::IsTrue(expected == result);

			//Select a pattern a(_,_"x*y"_) Expected: 12
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("_");
			arg_1.push_back("x*y");
			argType_1.push_back("all");
			argType_1.push_back("substring");
			qt_1.insertPattern("a", "assign", arg_1, argType_1);

			qt_1.insertSelect("a", "assign");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("12");
			Assert::IsTrue(expected_1 == result_1);

			//Select BOOLEAN pattern a(_,_"y+z"_) Expected: false
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("_");
			arg_2.push_back("y+z");
			argType_2.push_back("all");
			argType_2.push_back("substring");
			qt_2.insertPattern("a", "assign", arg_2, argType_2);
			
			qt_2.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("FALSE");
			Assert::IsTrue(expected_2 == result_2);
			
			//Select BOOLEAN pattern a("x",_) Expected: true
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("x");
			arg_3.push_back("_");
			argType_3.push_back("string");
			argType_3.push_back("all");
			qt_3.insertPattern("a", "assign", arg_3, argType_3);

			qt_3.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("TRUE");
			Assert::IsTrue(expected_3 == result_3);

			//Select BOOLEAN pattern a("x","x*y+z") Expected: true
			QueryTree qt_4;
			vector<string> arg_4;
			vector<string> argType_4;
			arg_4.push_back("x");
			arg_4.push_back("x*y+z");
			argType_4.push_back("string");
			argType_4.push_back("string");
			qt_4.insertPattern("a", "assign", arg_4, argType_4);

			qt_4.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_4 = (qt_4);
			list<string> result_4 = qe_4.evaluate();
			list<string> expected_4;
			expected_4.push_back("TRUE");
			Assert::IsTrue(expected_4 == result_4);

			//Select BOOLEAN pattern a("x","x*y") Expected: true
			QueryTree qt_5;
			vector<string> arg_5;
			vector<string> argType_5;
			arg_5.push_back("x");
			arg_5.push_back("x*y");
			argType_5.push_back("string");
			argType_5.push_back("substring");
			qt_5.insertPattern("a", "assign", arg_5, argType_5);

			qt_5.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_5 = (qt_5);
			list<string> result_5 = qe_5.evaluate();
			list<string> expected_5;
			expected_5.push_back("TRUE");
			Assert::IsTrue(expected_5 == result_5);
		}

		TEST_METHOD(evaluateWithNameProcedure) {
			/*
			Select p1 with p1.ProcName=p2.ProcName
			Select p1 with p1.ProcName=call.ProcName
			Select p1 with p1.ProcName=v.varName
			Select p1 with p1.ProcName="Second"
			*/

			//Select p1 with p1.ProcName=p2.ProcName
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("p1");
			arg.push_back("p2");
			argType.push_back("procedure");
			argType.push_back("procedure");
			qt.insertWith("withName", arg, argType);

			qt.insertSelect("p1", "procedure");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("Second");
			expected.push_back("Third");
			Assert::IsTrue(expected == result);

			//Select p1 with p1.ProcName=call.ProcName
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("p1");
			arg_1.push_back("call");
			argType_1.push_back("procedure");
			argType_1.push_back("call");
			qt_1.insertWith("withName", arg_1, argType_1);

			qt_1.insertSelect("p1", "procedure");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("Third");
			Assert::IsTrue(expected_1 == result_1);

			//Select p1 with p1.ProcName=v.varName
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("p1");
			arg_2.push_back("v");
			argType_2.push_back("procedure");
			argType_2.push_back("variable");
			qt_2.insertWith("withName", arg_2, argType_2);

			qt_2.insertSelect("p1", "procedure");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			Assert::IsTrue(expected_2 == result_2);

			//Select p1 with p1.ProcName="Second"
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("p1");
			arg_3.push_back("Second");
			argType_3.push_back("procedure");
			argType_3.push_back("string");
			qt_3.insertWith("withName", arg_3, argType_3);

			qt_3.insertSelect("p1", "procedure");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("Second");
			Assert::IsTrue(expected_3 == result_3);
		}

		TEST_METHOD(evaluateWithNameCall) {
			/*
			Select p with c.ProcName=p.ProcName
			Select call1 with call1.ProcName=call2.ProcName
			Select c with c.ProcName=v.varName
			Select c with c.ProcName="Third"
			*/

			//Select p with c.ProcName=p.ProcName
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("c");
			arg.push_back("p");
			argType.push_back("call");
			argType.push_back("procedure");
			qt.insertWith("withName", arg, argType);

			qt.insertSelect("p", "procedure");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("Third");
			Assert::IsTrue(expected == result);

			//Select call1 with call1.ProcName=call2.ProcName
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("call1");
			arg_1.push_back("call2");
			argType_1.push_back("call");
			argType_1.push_back("call");
			qt_1.insertWith("withName", arg_1, argType_1);

			qt_1.insertSelect("call1", "call");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("5");
			Assert::IsTrue(expected_1 == result_1);

			//Select c with c.ProcName=v.varName
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("c");
			arg_2.push_back("v");
			argType_2.push_back("call");
			argType_2.push_back("variable");
			qt_2.insertWith("withName", arg_2, argType_2);

			qt_2.insertSelect("p1", "procedure");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			Assert::IsTrue(expected_2 == result_2);

			//Select c with c.ProcName="Third"
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("c");
			arg_3.push_back("Third");
			argType_3.push_back("call");
			argType_3.push_back("string");
			qt_3.insertWith("withName", arg_3, argType_3);

			qt_3.insertSelect("c", "call");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("5");
			Assert::IsTrue(expected_3 == result_3);
		}

		TEST_METHOD(evaluateWithNameVariable) {
			/*
			Select v with v.varName=p.ProcName
			Select v with v.varName=call.ProcName
			Select v1 with v1.varName=v2.varName
			Select v with v.varName="x"
			*/

			//Select v with v.varName=p.ProcName
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("v");
			arg.push_back("p");
			argType.push_back("variable");
			argType.push_back("procedure");
			qt.insertWith("withName", arg, argType);

			qt.insertSelect("v", "variable");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			Assert::IsTrue(expected == result);

			//Select v with v.varName=call.ProcName
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("v");
			arg_1.push_back("call");
			argType_1.push_back("variable");
			argType_1.push_back("call");
			qt_1.insertWith("withName", arg_1, argType_1);

			qt_1.insertSelect("v", "variable");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			Assert::IsTrue(expected_1 == result_1);

			//Select v1 with v1.varName=v2.varName
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("v1");
			arg_2.push_back("v2");
			argType_2.push_back("variable");
			argType_2.push_back("variable");
			qt_2.insertWith("withName", arg_2, argType_2);

			qt_2.insertSelect("v1", "variable");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("i");
			expected_2.push_back("v");
			expected_2.push_back("x");
			expected_2.push_back("y");
			expected_2.push_back("z");
			Assert::IsTrue(expected_2 == result_2);

			//Select v with v.varName="x"
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("v");
			arg_3.push_back("x");
			argType_3.push_back("variable");
			argType_3.push_back("string");
			qt_3.insertWith("withName", arg_3, argType_3);

			qt_3.insertSelect("v", "variable");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("x");
			Assert::IsTrue(expected_3 == result_3);
		}

		TEST_METHOD(evaluateWithNameString) {
			/*
			Select p with "Seond"=p.ProcName
			Select call with "Third"=call.ProcName
			Select v with "x"=v.varName
			Select BOOLEAN with "x"="x"
			*/

			//Select p with "Seond"=p.ProcName
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("Second");
			arg.push_back("p");
			argType.push_back("string");
			argType.push_back("procedure");
			qt.insertWith("withName", arg, argType);

			qt.insertSelect("p", "procedure");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("Second");
			Assert::IsTrue(expected == result);

			//Select call with "Third"=call.ProcName
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("Third");
			arg_1.push_back("call");
			argType_1.push_back("string");
			argType_1.push_back("call");
			qt_1.insertWith("withName", arg_1, argType_1);

			qt_1.insertSelect("call", "call");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("5");
			Assert::IsTrue(expected_1 == result_1);

			//Select v with "x"=v.varName
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("x");
			arg_2.push_back("v");
			argType_2.push_back("string");
			argType_2.push_back("variable");
			qt_2.insertWith("withName", arg_2, argType_2);

			qt_2.insertSelect("v", "variable");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("x");
			Assert::IsTrue(expected_2 == result_2);

			//Select BOOLEAN with "x"="x"
			QueryTree qt_3;
			vector<string> arg_3;
			vector<string> argType_3;
			arg_3.push_back("x");
			arg_3.push_back("x");
			argType_3.push_back("string");
			argType_3.push_back("string");
			qt_3.insertWith("withName", arg_3, argType_3);

			qt_3.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_3 = (qt_3);
			list<string> result_3 = qe_3.evaluate();
			list<string> expected_3;
			expected_3.push_back("TRUE");
			Assert::IsTrue(expected_3 == result_3);
		}

		TEST_METHOD(evaluateWithNumberProg_line) {
			/*
			Select n1 with n1=n2
			Select n with n=8
			Select n with n=c.value
			*/
			//Select n1 with n1 = n2
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("n1");
			arg.push_back("n2");
			argType.push_back("prog_line");
			argType.push_back("prog_line");
			qt.insertWith("withNumber", arg, argType);

			qt.insertSelect("n1", "prog_line");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			expected.push_back("13");
			expected.push_back("14");
			expected.push_back("2"); 
			expected.push_back("3");
			expected.push_back("4");
			expected.push_back("5");
			expected.push_back("6");
			expected.push_back("7");
			expected.push_back("8");
			expected.push_back("9");
			Assert::IsTrue(expected == result);

			//Select n with n=8
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("n");
			arg_1.push_back("8");
			argType_1.push_back("prog_line");
			argType_1.push_back("number");
			qt_1.insertWith("withNumber", arg_1, argType_1);

			qt_1.insertSelect("n", "prog_line");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("8");
			Assert::IsTrue(expected_1 == result_1);

			//Select n with n=c.value
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("n");
			arg_2.push_back("c");
			argType_2.push_back("prog_line");
			argType_2.push_back("constant");
			qt_2.insertWith("withNumber", arg_2, argType_2);

			qt_2.insertSelect("n", "prog_line");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("1");
			expected_2.push_back("2");
			expected_2.push_back("5");
			Assert::IsTrue(expected_2 == result_2);

		}

		TEST_METHOD(evaluateWithNumberNumber) {
			/*
			Select n with 3=n
			Select BOOLEAN with 3=8
			Select c with 0=c.value
			*/
			//Select n with 3=n
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("3");
			arg.push_back("n");
			argType.push_back("number");
			argType.push_back("prog_line");
			qt.insertWith("withNumber", arg, argType);

			qt.insertSelect("n", "prog_line");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("3");
			Assert::IsTrue(expected == result);

			//Select BOOLEAN with 3=8
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("3");
			arg_1.push_back("8");
			argType_1.push_back("number");
			argType_1.push_back("number");
			qt_1.insertWith("withNumber", arg_1, argType_1);

			qt_1.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("FALSE");
			Assert::IsTrue(expected_1 == result_1);

			//Select c with 0=c.value
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("0");
			arg_2.push_back("c");
			argType_2.push_back("number");
			argType_2.push_back("constant");
			qt_2.insertWith("withNumber", arg_2, argType_2);

			qt_2.insertSelect("c", "constant");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("0");
			Assert::IsTrue(expected_2 == result_2);

		}

		TEST_METHOD(evaluateWithNumberConstant) {
			/*
			Select n with c.value=n
			Select BOOLEAN with c.value=0
			Select call with c.value=call.stmt#
			*/
			//Select n with c.value = n
			QueryTree qt;
			vector<string> arg;
			vector<string> argType;
			arg.push_back("c");
			arg.push_back("n");
			argType.push_back("constant");
			argType.push_back("prog_line");
			qt.insertWith("withNumber", arg, argType);

			qt.insertSelect("n", "prog_line");
			QueryEvaluator qe = (qt);
			list<string> result = qe.evaluate();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("5");
			Assert::IsTrue(expected == result);

			// Select BOOLEAN with c.value = 0
			QueryTree qt_1;
			vector<string> arg_1;
			vector<string> argType_1;
			arg_1.push_back("c");
			arg_1.push_back("0");
			argType_1.push_back("constant");
			argType_1.push_back("number");
			qt_1.insertWith("withNumber", arg_1, argType_1);

			qt_1.insertSelect("BOOLEAN", "boolean");
			QueryEvaluator qe_1 = (qt_1);
			list<string> result_1 = qe_1.evaluate();
			list<string> expected_1;
			expected_1.push_back("TRUE");
			Assert::IsTrue(expected_1 == result_1);

			//Select c with c.value=call.stmt#
			QueryTree qt_2;
			vector<string> arg_2;
			vector<string> argType_2;
			arg_2.push_back("c");
			arg_2.push_back("call");
			argType_2.push_back("constant");
			argType_2.push_back("call");
			qt_2.insertWith("withNumber", arg_2, argType_2);

			qt_2.insertSelect("c", "constant");
			QueryEvaluator qe_2 = (qt_2);
			list<string> result_2 = qe_2.evaluate();
			list<string> expected_2;
			expected_2.push_back("5");
			Assert::IsTrue(expected_2 == result_2);

		}

	};
}