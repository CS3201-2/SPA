#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(queryValidatorTest)
	{
	public:

		TEST_METHOD(isValidQuery)
		{
			vector<string> queries;
			vector<bool> queryResults;
			
			queries.push_back("assign a;variable b; Select a such that Uses(a, \"x\")");
			queryResults.push_back(true);
			
			queries.push_back("while w; assign a; Select w such that Follows(w, a)");
			queryResults.push_back(true);

			queries.push_back("procedure 1p; Select 1p with 1p.procName = \"SUN\"");
			queryResults.push_back(false);

			queries.push_back("assign a, a1, a2; while w; variable v, v1; if if1; "
				"Select a such that Modifies(a, v) pattern w(v, _ ) such that Follows(1,2) " 
				"pattern a1 (v1, _\"x+y\"_) such that Affects (a1, a2) with a2.stmt# = 20");
			queryResults.push_back(true);
			
			queries.push_back("assign a1, a2, a3; stmt s1,s2,s3; variable v1, v2, v3; "
				"Select <a1, s1, v2> such that Uses(5, \"y\") and Follows(3, 4) "
				"pattern a1(v2, _\"x + y\"_) such that Affects(a1, a2) with a2.stmt#  = 20"
				" such that Modifies(a3, v3) pattern a3(\"z\", _) such that Uses(s3, v1) "
				"and Modifies(s3, \"x\") and Follows(s1, s2) and Parent(3, s1) and Uses(s2, v1)");
			queryResults.push_back(true);

			queries.push_back("assign a1, a2; variable v; stmt s1, s2; Select s1 such that "
				"Follows(a1, a2) and Uses(a2, v) and Affects(a1, a2) with s1.stmt#  = 3 "
				"and s2.stmt# = 4");
			queryResults.push_back(true);

			queries.push_back("assign a; while w; variable v; Select a such that "
				"Modifies(a, v) pattern w(v, _ , _");
			queryResults.push_back(false);

			queries.push_back("assign a; variable v; Select v such that Modifies(a, v) "
				"pattern a(_,\"z + x + i\"))");
			queryResults.push_back(false);

			queries.push_back("assign \n a#1;variable \n #b; Select a     such that "
				"Uses(a, \"x\")");
			queryResults.push_back(false);

			queries.push_back(" ");
			queryResults.push_back(false);

			queries.push_back("assign a; Select a pattern a(_, _\"(f - d + b) - l\"_)");
			queryResults.push_back(true);

			queries.push_back("stmt s; Select s");
			queryResults.push_back(true);

			queries.push_back("Select BOOLEAN such that parent(1,2)");
			queryResults.push_back(true);

			queries.push_back("Select BOOLEAN with Affects(3, 9)");
			queryResults.push_back(false);

			queries.push_back("procedure 1p; Select 1p with 1p.procName = \"SUN\"");
			queryResults.push_back(false);

			queries.push_back("assign a; variable v; if ifs; variable v; Select "
				"a such that Modifies(a, v) pattern w(v,_)");
			queryResults.push_back(false);

			queries.push_back("assign   a,   c;Select   a     pattern a (\"a\", _\"x\"_) "
				"such that Uses  (a, \"x\") pattern a (\"a\", _\"x\"_)");
			queryResults.push_back(true);

			queries.push_back("assign a1; assign a2; Select _ such that Follows(a1, a2)");
			queryResults.push_back(false);

			queries.push_back(" ");
			queryResults.push_back(false);

			queries.push_back(" ");
			queryResults.push_back(false);

			for (int i = 0; i < queries.size(); i++) {
				QueryValidator q;
				Assert::AreEqual((bool)queryResults.at(i), q.isValidDecAndQuery(queries.at(i)));
			}
		}

	};
}