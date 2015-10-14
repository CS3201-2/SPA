#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "SPALog.h"

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
			str += "  z = z + (x + i);\n";
			str += "  y = z + 2;\n";
			str += "  x = x*y + z; }\n";
			str += "procedure Third{\n";
			str += "  z = 5;\n";
			str += "v = z; }\n";

			Parser parser = Parser();
			list<Statement> actualList = parser.prepareSourceList(str);
			
			list<Statement> expectedList;
			Statement temp = Statement();
			temp.setNumber(-1); 
			temp.setContent("procedureFirst{"); 
			temp.setType(procDeclarationStmt);
			expectedList.push_back(temp);
			
			temp.setNumber(1); 
			temp.setContent("x=2;");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);
			
			temp.setNumber(2);
			temp.setContent("z=3;");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(3); 
			temp.setContent("callSecond;}");
			temp.setType(procCallStmt);
			expectedList.push_back(temp);

			temp.setNumber(-1);
			temp.setContent("procedureSecond{");
			temp.setType(procDeclarationStmt);
			expectedList.push_back(temp);

			temp.setNumber(4); 
			temp.setContent("x=0;");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(5); 
			temp.setContent("i=5;"); 
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(6); 
			temp.setContent("whilei{");
			temp.setType(whileStmt);
			expectedList.push_back(temp);

			temp.setNumber(7); 
			temp.setContent("x=x+2*y;");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);
			
			temp.setNumber(8); 
			temp.setContent("callThird;"); 
			temp.setType(procCallStmt);
			expectedList.push_back(temp);

			temp.setNumber(9); 
			temp.setContent("i=i-1;}");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(10); 
			temp.setContent("ifxthen{"); 
			temp.setType(ifStmt);
			expectedList.push_back(temp);
			
			temp.setNumber(11); 
			temp.setContent("x=x+1;}");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(-1); 
			temp.setContent("else{");
			temp.setType(elseStmt);
			expectedList.push_back(temp);
			
			temp.setNumber(12); 
			temp.setContent("z=1;}");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(13);
			temp.setContent("z=z+(x+i);"); 
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(14);
			temp.setContent("y=z+2;");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(15); 
			temp.setContent("x=x*y+z;}"); 
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(-1);
			temp.setContent("procedureThird{");
			temp.setType(procDeclarationStmt);
			expectedList.push_back(temp);

			temp.setNumber(16);
			temp.setContent("z=5;");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			temp.setNumber(17);
			temp.setContent("v=z;}");
			temp.setType(assignmentStmt);
			expectedList.push_back(temp);

			Assert::IsTrue(actualList.size() == expectedList.size());

			list<Statement>::iterator it1 = actualList.begin();
			list<Statement>::iterator it2 = expectedList.begin();

			for (; it1 != actualList.end() && it2 != expectedList.end(); ++it1, ++it2) {
				Assert::IsTrue((*it1).isEqual(*it2));
				SPALog::log("1");
			}
		}
	};
}