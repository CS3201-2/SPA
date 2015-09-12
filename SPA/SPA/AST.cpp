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

ASTNode* AST::getRoot()
{
	return root;
}

void AST::acceptStatements(list<pair<int, string>> lst)
{
	stmts = lst;
	build();
}

bool AST::matchExpression(int index, string exp)
{
	checkIndex(index);
	if (exp == "_")
	{
		return true;
	}
	ASTNode* node = getNode(index);
	if (node->getNodeType() == "assignment")
	{
		if (isFullMatch(exp))
		{
			return AnotherExpressionTree::compareExpression(node->getRightChild(), exp);
		}
		else
		{
			string target = cutString(exp);
			return isPartialMatch(node->getRightChild(), target);
		}
	}
	else
	{
		throw "not an assignment";
	}
}

int AST::getFollowAfter(int index)
{
	checkIndex(index);
	ASTNode* nodeChosen = getNode(index);
	ASTNode* rightSibling = nodeChosen->getRightSibling();
	if (rightSibling == NULL)
	{
		return -1;
	}
	else
	{
		return rightSibling->getIndex();
	}
}

int AST::getFollowBefore(int index)
{
	checkIndex(index);
	ASTNode* nodeChosen = getNode(index);
	ASTNode* leftSibling = nodeChosen->getLeftSibling();
	if (leftSibling == NULL)
	{
		return -1;
	}
	else
	{
		return leftSibling->getIndex();
	}
}

int AST::getParent(int index)
{
	checkIndex(index);
	ASTNode* nodeChosed = getNode(index);
	ASTNode* parent = nodeChosed->getParent();
	if (parent == NULL)
	{
		throw "how can that be";
	}
	else
	{
		return parent->getIndex();
	}
}

list<int> AST::getChild(int index)
{
	checkIndex(index);
	list<int> tempLst;
	tempLst.clear();
	ASTNode* nodeChosen = getNode(index);
	if (!isContainer(nodeChosen))
	{
		return tempLst;
	}
	ASTNode* child = nodeChosen->getRightChild();
	while (child != NULL)
	{
		tempLst.push_back(child->getIndex());
		child = child->getRightSibling();
	}
	return tempLst;
}

ASTNode* AST::getNode(string s)
{
	return procPosition.at(s);
}

ASTNode* AST::getNode(int index)
{
	return statPosition[index];
}

void AST::build()
{
	statPosition.clear();
	statPosition.resize(stmts.size() + 1, NULL);
	procPosition.clear();
	root = constructAST(stmts);
}

ASTNode * AST::constructAST(list<pair<int, string>>& stmtList)
{
	list<pair<int, string>> subStmtList;
	stack <string> braces;
	bool wasEmptyBeforePop;

	ASTNode* returnNode = NULL;
	ASTNode* currentNode = NULL;
	ASTNode* tempNode = NULL;
	ASTNode* parentNode = NULL;
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
			/*cout << "sublist is : "<< endl;
			for (auto& x : subStmtList)
			{
				cout << x.first;
			}cout << endl;*/
			tempNode = constructAST(subStmtList);
			subStmtList.clear();
		}
		//stack was empty initially, stay at this level and append new node to the right sibling
		else if (braces.empty() && wasEmptyBeforePop) {
			tempNode = createNode(statement, index);
		}
		if (tempNode == NULL)
		{
		}
		else if (tempNode->getIndex() == -1)//it is a procedure statement;
		{
			procPosition.insert(pair<string, ASTNode*>
				(tempNode->getContent(), tempNode));
		}
		else//it is a normal statement
		{
			statPosition[tempNode->getIndex()] = tempNode;
		}
		if (returnNode == NULL) {
			returnNode = tempNode;
			currentNode = returnNode;
		}
		else// if (tempNode != NULL)
		{
			if (currentNode->getParent() == NULL && 
				currentNode->getLeftSibling() == NULL && 
				currentNode->getRightChild() == NULL &&
				parentNode == NULL)
			{
				parentNode = currentNode;
				currentNode->setRightChild(tempNode);
				tempNode->setParent(currentNode);
				currentNode = tempNode;
			}
			else
			{
				currentNode->setRightSibling(tempNode);
				tempNode->setLeftSibling(currentNode);
				tempNode->setParent(parentNode);
				currentNode = tempNode;
			}
		}
	}
	return returnNode;
}

ASTNode * AST::createNode(string str, int index)
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
	temp->setIndex(index);
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

void AST::checkIndex(int i)
{
	if (i > statPosition.size() || i < 0)
	{
		throw "index out of bound";
	}
}

ASTNode * AST::createProc(string str)
{
	return new ASTNode(getProcName(str), "procedure");
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

string AST::getProcName(string str)
{
	smatch matcher;
	string procName;
	bool found = regex_search(str, matcher, procDeclarationRegex);
	if (found)
	{
		procName = matcher[1];
	}
	return procName;
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

bool AST::isPartialMatch(ASTNode * node, string exp)
{
	if (node != NULL)
	{
		bool compare = AnotherExpressionTree::compareExpression(node, exp);
		if (compare)
		{
			return true;
		}
		else
		{
			ASTNode* right = node->getRightChild();
			ASTNode* left = node->getLeftChild();
			return isPartialMatch(right, exp) || isPartialMatch(left, exp);
		}
	}
	else
	{
		return false;
	}
}

bool AST::isContainer(ASTNode* node)
{
	return node->getNodeType() == "while" || node->getNodeType() == "if";
}

bool AST::isFullMatch(string str)
{
	return str[0] != '_';
}

string AST::cutString(string str)
{
	string subStr = str.substr(1, str.size() - 2);
	return subStr;
}
