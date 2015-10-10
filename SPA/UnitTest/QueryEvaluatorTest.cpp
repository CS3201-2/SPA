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
		
		TEST_METHOD(evaluate)
		{
			//whileList
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
			PKB::getPKBInstance()->addToCallStmtProcMap(5,-2);
			//varTable
			PKB::getPKBInstance()->insertVar("x");
			PKB::getPKBInstance()->insertVar("i");
			PKB::getPKBInstance()->insertVar("y");
			PKB::getPKBInstance()->insertVar("z");
			PKB::getPKBInstance()->insertVar("v");
			//PKB::getPKBInstance()->setVarTableReverse();
			//ProcTable
			PKB::getPKBInstance()->insertProc("Second");
			PKB::getPKBInstance()->insertProc("Third");
			//PKB::getPKBInstance()->setProcTableReverse();
			//Modifies
			list<int> varList;
			varList.push_back(4);
			varList.push_back(5);
			PKB::getPKBInstance()->setModifies(-2,varList);
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

			//PKB::getPKBInstance()->setModifiesReverse();

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

			//PKB::getPKBInstance()->setUsesReverse();
			//Follows
			PKB::getPKBInstance()->setFollows(-1, 9);
			PKB::getPKBInstance()->setFollows(1, 2);
			PKB::getPKBInstance()->setFollows(2, 3);
			PKB::getPKBInstance()->setFollows(3, 7);
			PKB::getPKBInstance()->setFollows(4, 5);
			PKB::getPKBInstance()->setFollows(5, 6);
			PKB::getPKBInstance()->setFollows(7, 10);
			PKB::getPKBInstance()->setFollows(10,11);
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
			PKB::getPKBInstance()->setCalls(-1,-2);
			PKB::getPKBInstance()->setCallsStar(-1, -2);

			DesignExtractor de = DesignExtractor();
			de.setFollowsStar();
			de.setParentStar();
			de.setReverseMap();
			
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
			Assert::AreEqual(expected.size(), result.size());

			
		}

		
	};
}