#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "DesignExtractor.h"
#include "QueryEvaluator.h"
#include "QueryValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(QueryProcessorIntegrationTest)
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
			PKB::getPKBInstance()->setCallsStar(-1, -2);

			DesignExtractor de = DesignExtractor();
			de.setFollowsStar();
			de.setParentStar();
			de.setReverseMap();

			//next and nextStar
			list<Statement> sourceList;
			Statement temp = Statement();
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
			SPALog::log("One Query Processor test complete!\n\n");
			PKB::DestroyInstance();
		}

		TEST_METHOD(evaluateWithProgLine)
		{
			
			string query = "prog_line n; Select BOOLEAN such that Modifies(n, \"x\") with n=1";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}
			
			list<string> expected;
			expected.push_back("true");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateParentPattern)
		{

			string query = "assign a; while w; Select a such that Parent(w, a) pattern a(\"x\",_)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("4");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateWithConstant)
		{

			string query = "constant c; stmt s; variable v; Select s such that Modifies(s, v) with s.stmt#=c.value";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("5");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesModifies)
		{

			string query = "while w; Select w such that Modifies(w, \"x\") and Modifies(4,_)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("3");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesUses)
		{

			string query = "assign a; Select a such that Modifies(a, \"x\") and Uses(a, \"x\")";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("12");
			expected.push_back("4");
			expected.push_back("8");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesParent)
		{

			string query = "assign a; stmt s; variable v; Select a such that Modifies(a, v) and Parent(s, a)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("4");
			expected.push_back("6");
			expected.push_back("8");
			expected.push_back("9");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesParentStar)
		{

			string query = "assign a; while w; Select a such that Modifies(a, \"x\") and Parent*(w, a)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("4");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesFollows)
		{

			string query = "assign a; while w; variable v; Select v such that Modifies(a, v) and Follows(a, w)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("i");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesFollowsStar)
		{

			string query = "stmt s; variable v; Select v such that Modifies(s, v) and Follows*(7, s)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("x");
			expected.push_back("y");
			expected.push_back("z");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesNext)
		{
			string query = "stmt s; variable v; Select v such that Modifies(s, v) and Next(7, s)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("x");
			expected.push_back("z");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesNextStar)
		{
			string query = "stmt s; Select s such that Modifies(s, \"y\") and Next*(7, s)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}

			list<string> expected;
			expected.push_back("11");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesIfPattern) {
			string query = "assign a; if ifs; variable v; Select a such that Modifies(a, v) pattern ifs(v,_,_)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}
			list<string> expected;
			expected.push_back("1");
			expected.push_back("4");
			expected.push_back("8");
			expected.push_back("12");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesWhilePattern) {
			string query = "assign a; while w; variable v; Select a such that Modifies(a, v) pattern w(v,_)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}
			list<string> expected;
			expected.push_back("2");
			expected.push_back("6");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesAssignPattern) {
			string query = "assign a; variable v; Select v such that Modifies(a, v) pattern a(_,_\"z+x+i\"_)";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}
			list<string> expected;
			expected.push_back("z");
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(evaluateModifiesNameWith) {
			string query = "call c; procedure p; variable v; Select v such that Modifies(p, v) with c.procName = p.procName";
			QueryValidator qv;
			list<string> result;
			if (qv.isValidDecAndQuery(query)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(qt);
				result = qe.evaluate();
			}
			else {
				SPALog::log("Wrong query");
			}
			list<string> expected;
			expected.push_back("v");
			expected.push_back("z");
			Assert::IsTrue(expected == result);
		}
	};
}