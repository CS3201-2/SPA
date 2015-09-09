#include <iostream>
#include <list>
#include <string>
#include <regex>
#include <stack>
#include <vector>
#include <map>
#include "ASTNode.h"
#include "AnotherExpressionTree.h"

using namespace std;

class AST
{
	map<string, ASTNode*> procPosition;
	vector<ASTNode*> statPosition;
	ASTNode* root;
	list<pair<int, string>> stmts;
public:
	AST();
	void build();
	ASTNode* getRoot();
	ASTNode* getNode(string);
	ASTNode* getNode(int);
	void acceptStatements(list<pair<int, string>>);
private:
	ASTNode* constructAST(list<pair<int, string>>&);
	ASTNode* createNode(string);
	ASTNode* createAssign(string);
	ASTNode* createProc(string);
	ASTNode* createWhile(string);
	ASTNode* createProg(string);
	void cutList(list<pair<int, string>>&);
	int countNumOfRightBraces(string);
	int countNumOfLeftBraces(string);
	int getTypeOfStatement(string);
};

