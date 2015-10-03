#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\SPA\QueryValidator.h"
#include "..\SPA\PKB.h"
//#include <list>
//#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SPA_UnitTest
{
	TEST_CLASS(testQueryValidator)
	{
	public:

		TEST_METHOD(isValidQuery)
		{
			QueryValidator q;
			vector<string> queries;
			queries.push_back("assign a;variable b; Select a such that Uses(a, \"x\")");
			queries.push_back("while w; assign a; Select w such that Follows(w, a)");
			Assert::IsTrue(q.isValidDecAndQuery(queries.at(0)));
			Assert::IsTrue(q.isValidDecAndQuery(queries.at(1)));
		}

	};
}