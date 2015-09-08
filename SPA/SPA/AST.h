#include <iostream>
#include <list>
#include <string>
#include <regex>
#include <stack>
#include "ASTNode.h"
#include "AnotherExpressionTree.h"

using namespace std;

class AST
{
	ASTNode* root;
	list<string> stmts;
public:
	AST();
	void build();
	ASTNode* getRoot();
	void getStatements(list<string>);
private:
	ASTNode* constructAST(list<string>& stmtList);
	ASTNode* createNode(string str);
	ASTNode* createAssign(string str);
	ASTNode* createProc(string str);
	ASTNode* createWhile(string str);
	ASTNode* createProg(string str);
	int countNumOfRightBraces(string);
	int countNumOfLeftBraces(string);
	int getTypeOfStatement(string);
};

