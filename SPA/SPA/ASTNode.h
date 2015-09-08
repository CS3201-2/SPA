#include <string>
#include <list>

using namespace std;

#ifndef ASTNode_H
#define ASTNode_H

class ASTNode
{
public:
	ASTNode(string);
	ASTNode(string, string);
	//ASTNode(NodeType);
	void setNodeType(string);
	string getContent();
	void setLeftChild(ASTNode*);
	void setRightChild(ASTNode*);
	void setLeftSibling(ASTNode*);
	void setRightSibling(ASTNode*);
	void setParent(ASTNode*);
	string getNodeType();

	//list<ASTNode> getChildren();
	ASTNode* getLeftSibling();
	ASTNode* getRightSibling();
	ASTNode* getRightChild();
	ASTNode* getLeftChild();
	ASTNode* getParent();

private:
	string _nodeType;
	string _nodeContent;
	ASTNode* _parent;
	ASTNode* _rightChild;
	ASTNode* _leftChild;
	ASTNode* _rightSibling;
	ASTNode* _leftSibling;
	//list<ASTNode> _children;
};
#endif
