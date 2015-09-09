#include "AST.h"

const regex assignmentRegex("(([[:alpha:]])([[:alnum:]]+)*)=(.*);\\}*");
const regex procDeclarationRegex("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{*");
const regex progDeclarationRegex("program(([[:alpha:]])([[:alnum:]]+)*)\\{*");
const regex whileRegex("while(([[:alpha:]])([[:alnum:]]+)*)\\{*");
const int assignmentStmt = 0;
const int procDeclarationStmt = 1;
const int procCallStmt = 2;
const int whileStmt = 3;
const int programStmt = 4;

AST::AST()
{
	root = NULL;
}

void AST::build()
{
	root = constructAST(stmts);
}

ASTNode* AST::getRoot()
{
	return root;
}

void AST::getStatements(list<pair<int, string>> lst)
{
	stmts = lst;
}

ASTNode * AST::constructAST(list<pair<int, string>>& stmtList)
{
	list<pair<int, string>> subStmtList;
	stack <string> braces;
	bool wasEmptyBeforePop;

	ASTNode* returnNode = NULL;
	ASTNode* currentNode = NULL;
	ASTNode* tempNode = NULL;
	for (list<pair<int, string>>::iterator it = stmtList.begin(); it != stmtList.end(); ++it) {
		int index = it->first;
		string statement = it->second;
		for (int i = 0; i < countNumOfLeftBraces(statement); ++i) {
			braces.push("{");
		}
		wasEmptyBeforePop = braces.empty();
		if (!wasEmptyBeforePop)
		{
			subStmtList.push_back(*it);
		}
		for (int i = 0; i < countNumOfRightBraces(statement); ++i) {
			braces.pop();
		}
		//stack was empty because of poping, going to the next level with fewer stmtList element
		if (braces.empty() && !wasEmptyBeforePop) {
			cutList(subStmtList);
			ASTNode* recursiveNode = constructAST(subStmtList);
			subStmtList.clear();
			tempNode = recursiveNode;
		}
		//stack was empty initially, stay at this level and append new node to the right sibling
		else if (braces.empty() && wasEmptyBeforePop) {
			tempNode = createNode(statement);
		}
		if (index == -1)//it is a procedure statement;
		{

		}
		if (returnNode == NULL) {
			returnNode = tempNode;
			currentNode = returnNode;
		}
		else
		{
			if (currentNode->getParent() == NULL && 
				currentNode->getLeftSibling() == NULL && 
				currentNode->getRightChild() == NULL)
			{
				currentNode->setRightChild(tempNode);
				tempNode->setParent(currentNode);
				currentNode = tempNode;
			}
			else //if (currentNode->getParent() != NULL || currentNode->getLeftSibling() != NULL)
			{
				currentNode->setRightSibling(tempNode);
				tempNode->setLeftSibling(currentNode);
				currentNode = tempNode;
			}
		}
	}
	return returnNode;
}

ASTNode * AST::createNode(string str)
{
	ASTNode* temp = NULL;
	switch (getTypeOfStatement(str)) {
	case assignmentStmt:
		temp = createAssign(str);
		break;
	case procDeclarationStmt:
		temp = createProc(str);
		break;
	case whileStmt:
		temp = createWhile(str);
		break;
	case programStmt:
		temp = createProg(str);
	default:
		throw "error";
	}
	return temp;
}

ASTNode * AST::createAssign(string str)
{
	smatch matcher;
	string variable;
	string expression;
	bool found = regex_search(str, matcher, assignmentRegex);
	if (found)
	{
		variable = matcher[1];
		expression = matcher[4];
	}
	AnotherExpressionTree et;
	et.build(expression);
	ASTNode * expressionTree = et.peek();
	ASTNode * variableNode = new ASTNode(variable, "variable");
	ASTNode * assignNode = new ASTNode("assignment");
	assignNode->setLeftChild(variableNode);
	assignNode->setRightChild(expressionTree);
	return assignNode;
}

ASTNode * AST::createProg(string str)
{
	smatch matcher;
	string progName;
	bool found = regex_search(str, matcher, progDeclarationRegex);
	if (found)
	{
		progName = matcher[1];
	}
	return new ASTNode(progName, "program");
}

void AST::cutList(list<pair<int, string>>& lst)
{
	int tempIndex;
	string tempStat;
	tempIndex = lst.front().first;
	tempStat = lst.front().second;
	tempStat = tempStat.substr(0, tempStat.size() - 1);
	lst.pop_front();
	pair<int, string> newFrontPair(tempIndex, tempStat);
	lst.push_front(newFrontPair);

	tempIndex = lst.back().first;
	tempStat = lst.back().second;
	tempStat = tempStat.substr(0, tempStat.size() - 1);
	lst.pop_back();
	pair<int, string> newBackPair(tempIndex, tempStat);
	lst.push_back(newBackPair);
}

ASTNode * AST::createProc(string str)
{
	smatch matcher;
	string procName;
	bool found = regex_search(str, matcher, procDeclarationRegex);
	if (found)
	{
		procName = matcher[1];
	}
	return new ASTNode(procName, "procedure");
}

ASTNode * AST::createWhile(string str)
{
	smatch matcher;
	string variable;
	bool found = regex_search(str, matcher, whileRegex);
	if (found)
	{
		variable = matcher[1];
	}
	ASTNode* varNode = new ASTNode(variable, "variable");
	ASTNode* whileNode = new ASTNode("while");
	whileNode->setLeftChild(varNode);
	return whileNode;
}

int AST::countNumOfRightBraces(string str)
{
	return std::count(str.begin(), str.end(), '}');
}

int AST::countNumOfLeftBraces(string str)
{
	return std::count(str.begin(), str.end(), '{');
}

int AST::getTypeOfStatement(string str)
{
	if (regex_match(str, assignmentRegex)) {
		return assignmentStmt;
	}
	else if (regex_match(str, procDeclarationRegex)) {
		return procDeclarationStmt;
	}
	else if (regex_match(str, whileRegex)) {
		return whileStmt;
	}
	else if (regex_match(str, progDeclarationRegex)) {
		return programStmt;
	}
	else
	{
		throw "error";
	}
}

