#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\SPA\Parser.h"
#include <list>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SPA_UnitTest
{		
	TEST_CLASS(testParser)
	{
	public:
		
		TEST_METHOD(trim)
		{
			Parser parser;
			
			string expected = "akshdbdjhdfbasdakhsbadkshbdhfsadhvhj";

			string str = "akshdbdjhdfb  asdakhsb \n adkshbdhf\nsadhvhj";

			parser.trim(str);

			Assert::AreEqual( expected, str );
		}


		TEST_METHOD(addNewLineString)
		{
			Parser parser;
			string expected = "procedure seconde{@";
			string str = "procedure seconde{";

			parser.addNewLineString(str);

			Assert::AreEqual(expected, str);

		}

	};
}