#include <string>
#include <list>


using namespace std;

#ifndef ASTNode_H
#define ASTNode_H

class ASTNode
{
public:
	ASTNode(string, string);
	//ASTNode(NodeType);
	string getNodeType();
	void setNodeType(string);
	string getNodeContent();
	void setNodeContent(string);
	//string getContent();
	void setLeftChild(ASTNode*);
	void setRightChild(ASTNode*);
	void setLeftSibling(ASTNode*);
	void setRightSibling(ASTNode*);
	void setParent(ASTNode*);

	//list<ASTNode> getChildren();
	ASTNode* getLeftSibling();
	ASTNode* getRightSibling();
	ASTNode* getRightChild();
	ASTNode* getLeftChild();
	ASTNode* getParent();

private:
	string _nodeContent;
	string _nodeType;
	//string _nodeContent;
	ASTNode* _parent;
	ASTNode* _rightChild;
	ASTNode* _leftChild;
	ASTNode* _rightSibling;
	ASTNode* _leftSibling;
	//list<ASTNode> _children;
};
#endif
