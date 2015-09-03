#include "ExpressionTree.h"

ExpressionTree::ExpressionTree(string str)
{
	constructTree(str);
}

ASTNode ExpressionTree::getRoot()
{
	return _root;
}

void ExpressionTree::constructTree(string str)
{
	string suffix = getSuffix(str);
	buildTree(suffix);
}

string ExpressionTree::getSuffix(string str) 
//assert(str is without space)
{
	unordered_map<char, int> inStack;
	unordered_map<char, int> outStack;

	inStack.emplace('(', 1);	outStack.emplace('(', 6);
	inStack.emplace('+', 3);	outStack.emplace('+', 2);
	inStack.emplace('-', 3);	outStack.emplace('-', 2);
	inStack.emplace('*', 5);	outStack.emplace('*', 4);
	inStack.emplace('/', 5);	outStack.emplace('/', 4);
	inStack.emplace(')', 6);	outStack.emplace(')', 1);
	string subject = "#" + str;
	string numberBuffer = "";
	string output = "";
	int i;
	for (i = 0; i < subject.length; i++)
	{
		switch (outStack[subject[i]]) {
		case 0:
			output = output + subject[i];
			break;
		}
	}
	return;
}

void ExpressionTree::buildTree(string str)
{

}
