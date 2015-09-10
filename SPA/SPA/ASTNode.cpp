#include "ASTNode.h"

using namespace std;

//Constructor
ASTNode::ASTNode(string type)
{
	setNodeType(type);
	_index = 0;
	_parent = NULL;
	_rightChild = NULL;
	_leftChild = NULL;
	_leftSibling = NULL;
	_rightSibling = NULL;
}

ASTNode::ASTNode(string content, string type)
{
	_nodeContent = content;
	setNodeType(type);
	_index = 0;
	_parent = NULL;
	_rightChild = NULL;
	_leftChild = NULL;
	_leftSibling = NULL;
	_rightSibling = NULL;
}

string ASTNode::getNodeType() {
	return _nodeType;
}

void ASTNode::setNodeType(string nodeType)
{
	_nodeType = nodeType;
}

string ASTNode::getContent()
{
	return _nodeContent;
}

int ASTNode::getIndex()
{
	return _index;
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

void ASTNode::setIndex(int i)
{
	_index = i;
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




