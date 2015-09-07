#include<string>
#include"CppUnitTest.h"
#include "stdafx.h"
#include "Suffix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SuffixTest
{
	TEST_CLASS(Suffix)
	{
	public:

		TEST_METHOD(getSuffix)
		{
			Suffix testSuffix;

			string expression1 = "1+2+3+4+5";
			testSuffix.Suffix::acceptExpression(expression1);
			Assert::AreEqual(1, 1);
			
		}
	};
}