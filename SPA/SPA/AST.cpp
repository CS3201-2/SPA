#include "AST.h"

using namespace std;

const string kErrorType = "Error: the type of this line is ambiguous!";

//Constructor
AST::AST(string initialLine)
{
	_root = &programNode;
	_nodeBefore = NULL;
	_nodeInProcess.push(programNode);
}

void AST::updateAST(string line)
{
	if (line is "}")
	{
		_nodeBefore = NULL;
		_nodeInProcess.pop();
	}
	else
	{
		NodeType type = getLineType(line);
		ASTNode node;
		ASTNode* nodePointer = &node;
		switch (type)
		{
		case procedure:
			createProcNode(nodePointer, line);
			break;
		case assign:
			createAssignNode(nodePointer, line);
			break;
		case call:
			createCallNode(nodePointer);
			break;
		case whilestmt:
			createWhileNode(nodePointer);
			break;
		case ifstmt:
			createIfNode(nodePointer);
			break;
		default:
			throw kErrorType;
			break;
		}
		_nodeInProcess.top().addChildren(node);
		if (_nodeBefore != NULL)
		{
			(*_nodeBefore).setSibling(node);
			_nodeBefore = &node;
		}
		if (line contains "{")
		{
			_nodeInProcess.push(node);
		}
	}
}

ASTNode AST::getRoot()
{
	return *_root;
}

NodeType AST::getLineType(string line)
{
	return ;
}

void AST::createProcNode(ASTNode* ptr)
{
	*ptr = ASTNode(procedure, GetLineName!!!);
}

void AST::createAssignNode(ASTNode* ptr, string line)
{
	*ptr = ASTNode(assign, NULL);
	ASTNode variableNode(variable, getVariable(line));
	(*ptr).addChildren(variableNode);
	addExpression(ptr, getExpression(line));
}

void AST::createCallNode(ASTNode* ptr, string line)
{
	*ptr = ASTNode(call, GetLineName!!!);
}

void AST::createWhileNode(ASTNode* ptr, string line)
{
	*ptr = ASTNode(whilestmt, NULL);
	ASTNode conditionNode(variable, getVariable(line));
	ASTNode stmtLstNode(statementLst, NULL);
	(*ptr).addChildren(conditionNode);
	(*ptr).addChildren(stmtLstNode);
}

void AST::createIfNode(ASTNode* ptr, string line)
{
	*ptr = ASTNode(ifstmt, NULL);
	ASTNode conditionNode(variable, getVariable(line));
	ASTNode thenNode(statementLst, "then");
	ASTNode elseNode(statementLst, "else");
	(*ptr).addChildren(conditionNode);
	(*ptr).addChildren(thenNode);
	(*ptr).addChildren(thenNode);
}

void AST::addExpression(ASTNode* ptr, Expression exp)
{
	(*ptr).addChildren(exp.getRoot());
}