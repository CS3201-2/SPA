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
#ifndef AST_H
#define AST_H
class AST
{
	map<string, ASTNode*> procPosition;
	vector<ASTNode*> statPosition;
	ASTNode* root;
	list<pair<int, string>> stmts;
public:
	AST();
	void acceptStatements(list<pair<int, string>>);
	bool matchExpression(int, string);
	int getFollowAfter(int);
	int getFollowBefore(int);
	int getParent(int);
	list<int> getChild(int);

private:
	void build();
	ASTNode* getRoot();
	ASTNode* getNode(string);
	ASTNode* getNode(int);
	ASTNode* constructAST(list<pair<int, string>>&);
	ASTNode* createNode(string, int);
	ASTNode* createAssign(string);
	ASTNode* createProc(string);
	ASTNode* createWhile(string);
	ASTNode* createProg(string);
	string getProcName(string);
	void cutList(list<pair<int, string>>&);
	void checkIndex(int);
	int countNumOfRightBraces(string);
	int countNumOfLeftBraces(string);
	int getTypeOfStatement(string);
	bool isPartialMatch(ASTNode*, string);
	bool isContainer(ASTNode*);
	bool isFullMatch(string);
	string cutString(string);
};
#endif
