#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(queryTreeTest) 
	{
	public:

		TEST_METHOD(getSuchThatSize)
		{
			QueryTree qt;
			vector<string> arrVar;
			vector<string> arrType;
			arrVar.push_back("s");
			arrVar.push_back("x");
			arrType.push_back("stmt");
			arrType.push_back("string");
			qt.insertSuchThat("modifies", arrVar, arrType);
			//Assert::AreEqual(1, qt.getSuchThatSize());
			//Assert::AreEqual(0, qt.getSuchThatConstSize());
		}

		TEST_METHOD(getSelectSize)
		{	
			QueryTree qt;
			qt.insertSelect("1", "2");
			qt.insertSelect("3", "4");
			//Assert::AreEqual(2, qt.getSelectSize());
		}
	};
}