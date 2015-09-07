#include "AST.h"

using namespace std;

const string kErrorType = "Error: the type of this line is ambiguous!";
const regex procDeclaration("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{");
const regex elseStmt("else\\{");
const regex assignment("(([[:alpha:]])([[:alnum:]]+)*)=(.*);\\}*");
const regex procCall("call(([[:alpha:]])([[:alnum:]]+)*);\\}*");
const regex whileStmt("while(([[:alpha:]])([[:alnum:]]+)*)\\{");
const regex ifStmt("if(([[:alpha:]])([[:alnum:]]+)*)then\\{");

//Constructor
AST::AST(list<pair<int, string>> codeList)
{
	_code = codeList;
	_nodeBefore = NULL;
	string programNameLine = (*codeList.begin()).second;
	ASTNode programNode = createProgNode(programNameLine);
	_nodeInProcess.push(programNode);
}

void AST::constructTree()
{
	for (list<pair<int, string>>::iterator it = _code.begin(); it != _code.end(); ++it)
	{
		string line = (*it).second;
		updateAST(line);
	}
}

void AST::updateAST(string line)
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
			createCallNode(nodePointer, line);
			break;
		case whilestmt:
			createWhileNode(nodePointer, line);
			break;
		case ifstmt:
			createIfNode(nodePointer, line);
			break;
		case elf:
			break;
		default:
			throw kErrorType;
			break;
		}
		if (containsLeftCurlyBracket(line))
		{
			_nodeInProcess.push(node);
			/*
			if (regex_match(line, elseStmt))
			{
				ASTNode elseNode(statementLst, "else");
				_nodeInProcess.top().addChildren(elseNode);
				_nodeInProcess.
			}
			else
			{
				_nodeInProcess.push(node);
			}*/
		}
		_nodeInProcess.top().addChildren(node);
		if (_nodeBefore != NULL)
		{
			(*_nodeBefore).setSibling(node);
		}
		_nodeBefore = &node;
		if (containsRightCurlyBracket(line))
		{
			_nodeBefore = NULL;
			_nodeInProcess.pop();
		}
		
}

ASTNode AST::getRoot()
{
	return *_root;
}

NodeType AST::getLineType(string str)
{
	if (regex_match(str, assignment)) {
		return NodeType::assign;
	}
	else if (regex_match(str, procDeclaration)) {
		return NodeType::procedure;
	}
	else if (regex_match(str, procCall)) {
		return NodeType::call;
	}
	else if (regex_match(str, whileStmt)) {
		return NodeType::whilestmt;
	}
	else if (regex_match(str, ifStmt)) {
		return NodeType::ifstmt;
	}
	else if (regex_match(str, elseStmt)) {
		return NodeType::elf;
	}
	else {
		return NodeType::default;
	}
}

ASTNode AST::createProgNode(string str)
{
	smatch matcher;
	bool found = regex_search(str, matcher, procDeclaration);
	if (found)
	{
		string name = matcher[1];
		return ASTNode(NodeType::program, name);
	}
	else
	{
		throw kErrorType;
	}
}

void AST::createProcNode(ASTNode* ptr, string str)
{
	smatch matcher;
	bool found = regex_search(str, matcher, procDeclaration);
	if (found)
	{
		string name = matcher[1];
		*ptr = ASTNode(NodeType::procedure, name);
	}
	else
	{
		throw kErrorType;
	}
}

void AST::createAssignNode(ASTNode* ptr, string line)
{
	*ptr = ASTNode(assign, NULL);
	ASTNode variableNode(variable, getVariable(line));
	(*ptr).addChildren(variableNode);
	addExpression(ptr, getExpression(line));
}

bool AST::containsRightCurlyBracket(string str)
{
	int found = str.find_first_of('{');
	if (found == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool AST::containsLeftCurlyBracket(string str)
{
	int found = str.find_first_of('}');
	if (found == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

string AST::getVariable(string str)
{
	smatch matcher;
	string variable;
	bool found = regex_search(str, matcher, assignment);
	if (found)
	{
		variable = matcher[1];
	}
	found = regex_search(str, matcher, whileStmt);
	if (found)
	{
		variable = matcher[1];
	}
	found = regex_search(str, matcher, ifStmt);
	if (found)
	{
		variable = matcher[1];
	}
	return variable;
}

string AST::getExpression(string str)
{
	int index = str.find_first_of('=');
	return str.substr(index + 1, str.size()-index-1);
}

void AST::createCallNode(ASTNode* ptr, string line)
{
	smatch matcher;
	bool found = regex_search(line, matcher, procCall);
	string name;
	if (found)
	{
		name = matcher[1];
	}
	else
	{
		throw kErrorType;
	}
	*ptr = ASTNode(call, name);
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

void AST::addExpression(ASTNode* ptr, string exp)
{
	ExpressionTree et(exp);
	(*ptr).addChildren(et.getRoot());
}