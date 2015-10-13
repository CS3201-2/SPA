#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(parserTest)
	{
	public:
		TEST_METHOD(testPrepareSourceList) {
			string str;
			str = "procedure First{\n";
			str += "  x=  2;\n";
			str += "  z =3;\n";
			str += "  call Second;}\n";
			str += "procedure Second{\n";
			str += "  x = 0;\n";
			str += "  i = 5;\n";
			str += "  while i{\n";
			str += "     x = x + 2 * y;\n";
			str += "     call Third;\n";
			str += "     i = i - 1; }\n";
			str += "  if x then{\n";
			str += "     x = x + 1; }\n";
			str += "  else {\n";
			str += "     z = 1;\n";
			str += "  }\n";
			str += "  z = z + x + i;\n";
			str += "  y = z + 2;\n";
			str += "  x = x*y + z; }\n";
			str += "procedure Third{\n";
			str += "  z = 5;\n";
			str += "v = z; }\n";

			Parser parser = Parser();
			list<pair<int, string>> actualList = parser.prepareSourceList(str);
			
			list<pair<int, string>> expectedList;
			pair<int, string> temp;			
			temp.first = -1; temp.second = "procedureFirst{"; expectedList.push_back(temp);
			temp.first = 1; temp.second = "x=2;"; expectedList.push_back(temp);
			temp.first = 2; temp.second = "z=3;"; expectedList.push_back(temp);
			temp.first = 3; temp.second = "callSecond;}"; expectedList.push_back(temp);
			temp.first = -1; temp.second = "procedureSecond{"; expectedList.push_back(temp);
			temp.first = 4; temp.second = "x=0;"; expectedList.push_back(temp);
			temp.first = 5; temp.second = "i=5;"; expectedList.push_back(temp);
			temp.first = 6; temp.second = "whilei{"; expectedList.push_back(temp);
			temp.first = 7; temp.second = "x=x+2*y;"; expectedList.push_back(temp);
			temp.first = 8; temp.second = "callThird;"; expectedList.push_back(temp);
			temp.first = 9; temp.second = "i=i-1;}"; expectedList.push_back(temp);
			temp.first = 10; temp.second = "ifxthen{"; expectedList.push_back(temp);
			temp.first = 11; temp.second = "x=x+1;}"; expectedList.push_back(temp);
			temp.first = -1; temp.second = "else{"; expectedList.push_back(temp);
			temp.first = 12; temp.second = "z=1;}"; expectedList.push_back(temp);
			temp.first = 13; temp.second = "z=z+x+i;"; expectedList.push_back(temp);
			temp.first = 14; temp.second = "y=z+2;"; expectedList.push_back(temp);
			temp.first = 15; temp.second = "x=x*y+z;}"; expectedList.push_back(temp);
			temp.first = -1; temp.second = "procedureThird{"; expectedList.push_back(temp);
			temp.first = 16; temp.second = "z=5;"; expectedList.push_back(temp);
			temp.first = 17; temp.second = "v=z;}"; expectedList.push_back(temp);

			Assert::IsTrue(actualList == expectedList);
		}
	};
}