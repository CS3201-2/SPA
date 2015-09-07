#include"StackNode.h"
#include"Suffix.h"
#include<iostream>
#include<regex>

using namespace std;
#ifndef AnotherExpressionTree_H
#define AnotherExpressionTree_H



class AnotherExpressionTree
{

private:

	StackNode *top;

public:
	AnotherExpressionTree();
	void clear();
	void push(ASTNode* ptr);
	ASTNode* pop();
	ASTNode* peek();
	void insert(string x);
	void build(string);
	bool isOperator(string str);
	bool isNumber(string str);
};
#endif
