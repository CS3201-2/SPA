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
			arrVar.push_back("1");
			arrVar.push_back("2");
			arrType.push_back("variable");
			arrType.push_back("variable");
			qt.insertSuchThat("Modifies", arrVar, arrType);
			Assert::AreEqual(1, qt.getSuchThatSize());
		}

		TEST_METHOD(getSelectSize)
		{	
			QueryTree qt;
			qt.insertSelect("1", "2");
			qt.insertSelect("3", "4");
			Assert::AreEqual(2, qt.getSelectSize());
		}
	};
}