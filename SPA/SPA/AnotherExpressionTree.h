#include"StackNode.h"
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
	ASTNode* peek();
	void build(string);

private:
	AnotherExpressionTree();
	void clear();
	void push(ASTNode* ptr);
	void insert(string x);
	bool isOperator(string str);
	bool isNumber(string str);
	ASTNode* pop();
};
#endif
