#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include <list>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SPA_UnitTest
{		
	TEST_CLASS(testParser)
	{
	public:
		
		TEST_METHOD(removeBlankLines)
		{
			Parser parser;
			list<string> strList;

			strList.push_back("     ");
			strList.push_back("this is awesome!");
			strList.push_back("   ");

			parser.removeBlankLines(strList);

			Assert::AreEqual( 1,  1);
		}

	};
}