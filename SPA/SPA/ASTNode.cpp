#include "ASTNode.h"

using namespace std;

//Constructor
ASTNode::ASTNode(string type)
{
	setNodeType(type);
	setNodeContent("NULL");
	_parent = NULL;
	_rightChild = NULL;
	_leftChild = NULL;
	_leftSibling = NULL;
	_leftSibling = NULL;
}

ASTNode::ASTNode(string content, string type)
{
	setNodeType(type);
	setNodeContent(content);
	_parent = NULL;
	_rightChild = NULL;
	_leftChild = NULL;
	_leftSibling = NULL;
	_leftSibling = NULL;
}

string ASTNode::getNodeType()
{
	return _nodeType;
}

void ASTNode::setNodeType(string nodeType)
{
	_nodeType = nodeType;
}

string ASTNode::getNodeContent()
{
	return _nodeContent;
}

void ASTNode::setNodeContent(string nodeContent)
{
	_nodeContent = nodeContent;
}

void ASTNode::setLeftChild(ASTNode* node) {
	_leftChild = node;
}

void ASTNode::setRightChild(ASTNode* node) {
	_rightChild = node;
}

void ASTNode::setLeftSibling(ASTNode* node) {
	_leftSibling = node;
}

void ASTNode::setRightSibling(ASTNode* node) {
	_rightSibling = node;
}

void ASTNode::setParent(ASTNode* node) {
	_parent = node;
}

ASTNode* ASTNode::getParent() {
	return _parent;
}

ASTNode* ASTNode::getRightChild() {
	return _rightChild;
}

ASTNode* ASTNode::getLeftChild() {
	return _leftChild;
}

ASTNode* ASTNode::getRightSibling() {
	return _rightSibling;
}

ASTNode* ASTNode::getLeftSibling() {
	return _leftSibling;
}