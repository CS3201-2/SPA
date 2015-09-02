#include "AST.h"

using namespace std;

const string kErrorType = "Error: the type of this line is ambiguous!";

//Constructor
AST::AST(string initialLine)
{
	_root = NULL;
	_nodeInProcess.push(startOfProgram);
}

void AST::updateAST(string line)
{
	NodeType type = getLineType(line);
	ASTNode node;
	ASTNode* nodePointer = &node;
	switch (type)
	{
	case procedure:
		createProcNode(nodePointer);
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
	*ptr = 
}

void AST::updateCall(string line)
{

}

void AST::updateWhile(string line)
{

}

void AST::updateIf(string line)
{

}