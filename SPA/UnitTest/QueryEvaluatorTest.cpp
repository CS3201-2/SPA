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

		TEST_METHOD_INITIALIZE(buidPKB) {
				PKB::getPKBInstance()->addStmtToList(3, 3);
				//assignList
				PKB::getPKBInstance()->addStmtToList(1, 0);
				PKB::getPKBInstance()->addStmtToList(2, 0);
				PKB::getPKBInstance()->addStmtToList(4, 0);
				PKB::getPKBInstance()->addStmtToList(6, 0);
				PKB::getPKBInstance()->addStmtToList(8, 0);
				PKB::getPKBInstance()->addStmtToList(9, 0);
				PKB::getPKBInstance()->addStmtToList(10, 0);
				PKB::getPKBInstance()->addStmtToList(11, 0);
				PKB::getPKBInstance()->addStmtToList(12, 0);
				PKB::getPKBInstance()->addStmtToList(13, 0);
				PKB::getPKBInstance()->addStmtToList(14, 0);
				//callList
				PKB::getPKBInstance()->addStmtToList(5, 2);
				//ifList
				PKB::getPKBInstance()->addStmtToList(7, 4);
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
				PKB::getPKBInstance()->setCallsStar(-1, -2);

				DesignExtractor de = DesignExtractor();
				de.setFollowsStar();
				de.setParentStar();
				de.setReverseMap();

				//next and nextStar
				list<pair<int, string>> sourceList;
				pair<int, string> temp;
				temp.first = -1; temp.second = "procedureSecond{"; sourceList.push_back(temp);
				temp.first = 1; temp.second = "x=0;"; sourceList.push_back(temp);
				temp.first = 2; temp.second = "i=5;"; sourceList.push_back(temp);
				temp.first = 3; temp.second = "whilei{"; sourceList.push_back(temp);
				temp.first = 4; temp.second = "x=x+2*y;"; sourceList.push_back(temp);
				temp.first = 5; temp.second = "callThird;"; sourceList.push_back(temp);
				temp.first = 6; temp.second = "i=i-1;}"; sourceList.push_back(temp);
				temp.first = 7; temp.second = "ifxthen{"; sourceList.push_back(temp);
				temp.first = 8; temp.second = "x=x+1;}"; sourceList.push_back(temp);
				temp.first = -1; temp.second = "else{"; sourceList.push_back(temp);
				temp.first = 9; temp.second = "z=1;}"; sourceList.push_back(temp);
				temp.first = 10; temp.second = "z=z+x+i;"; sourceList.push_back(temp);
				temp.first = 11; temp.second = "y=z+2;"; sourceList.push_back(temp);
				temp.first = 12; temp.second = "x=x*y+z;}"; sourceList.push_back(temp);
				temp.first = -1; temp.second = "procedureThird{"; sourceList.push_back(temp);
				temp.first = 13; temp.second = "z=5;"; sourceList.push_back(temp);
				temp.first = 14; temp.second = "v=z;}"; sourceList.push_back(temp);

				PKB::getPKBInstance()->buildCFG(sourceList);
			
		}

		TEST_METHOD_CLEANUP(clearup) {
			SPALog::log("One evaluate test complete!\n\n");
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
			expected.push_back("3");
			expected.push_back("4");
			expected.push_back("7");
			expected.push_back("8");
			expected.push_back("12");
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
			expected_2.push_back("x");
			expected_2.push_back("i");
			expected_2.push_back("y");
			expected_2.push_back("z");
			expected_2.push_back("v");
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
			expected_4.push_back("z");
			expected_4.push_back("v");
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
			expected_9.push_back("false");
			//Assert::IsTrue(expected_9 == result_9);
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
			expected.push_back("3");
			expected.push_back("4");
			expected.push_back("7");
			expected.push_back("8");
			expected.push_back("10");
			expected.push_back("12");
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
			expected_2.push_back("x");
			expected_2.push_back("i");
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
		}
		
		TEST_METHOD(evaluateParent) {
			/*
			Select s such that Parent(7, s) Expected: 8,9
			Select s such that Parent(5, s) Expected:
			Select w such that Parent(w, _) Expected: 3
			Select ifs such that Parent( ifs , 8) Expected:7
			Select a such that Parent(w, a) Expected: 4,6
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
		}

		TEST_METHOD(evaluateFollows) {
			/*
			Select s such that Follows(1, s) Expected: 2
			Select s such that Follows(6, s) Expected:
			Select w such that Follows(w, _) Expected: 3
			Select ifs such that Follows( ifs , 10) Expected:7
			Select s such that Follows(w, s) Expected:7 
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
		}

		TEST_METHOD(evaluateNext) {
			/*
			Select s such that Next(3, s) Expected: 4,7
			Select s such that Next(100, s) Expected:
			Select w such that Next(w, _) Expected: 3
			Select s such that Next( c , s) Expected:6
			Select ifs such that Next(_, ifs) Expected:7
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
		}
		
	};
}