#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h"
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
			list<string> queries;

			queries.push_back("assign a;variable b; Select a such that Uses(a, \"x\")");
			q.isValidQuery(queries);
		}

	};
}