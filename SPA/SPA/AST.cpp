#include "AST.h"

using namespace std;

const string kErrorType = "Error: the type of this line is ambiguous!";

//Constructor
AST::AST(string initialLine)
{
	_root = NULL;
	_nodeInProcess.push(startOfProgram);
	_nodeInProcess.push(startOfFirstProcedure);
}

void AST::updateAST(string line)
{
	NodeType type = getLineType(line);
	switch (type)
	{
	case procedure:
		updateProcedure(line);
		break;
	case assign:
		updateAssign(line);
		break;
	case call:
		updateCall(line);
		break;
	case whilestmt:
		updateWhile(line);
		break;
	case ifstmt:
		updateIf(line);
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

void AST::updateProcedure(string line)
{
	ASTNode newNode(procedure, GetLineName!!!);

}

void AST::updateAssign(string line)
{

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