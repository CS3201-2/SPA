#include"StackNode.h"
#include"Prefix.h"
#include"Suffix.h"
#include<iostream>
#include<regex>

using namespace std;
#ifndef AnotherExpressionTree_H
#define AnotherExpressionTree_H



class AnotherExpressionTree
{
	StackNode *top;

public:
	AnotherExpressionTree();
	ASTNode* peek();
	void build(string);
	static bool compareExpression(ASTNode*, string);
	static bool isOperator(ASTNode*);
private:
	void clear();
	void push(ASTNode* ptr);
	void insert(string x);
	bool isOperator(string str);
	bool isNumber(string str);
	ASTNode* pop();
	static string interpreteExpression(ASTNode*);
};
#endif
