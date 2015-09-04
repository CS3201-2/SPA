#include <string>
#include <list>
#include "NodeTypeEnum.h"

using namespace std;

#ifndef ASTNode_H
#define ASTNode_H

class ASTNode
{
	NodeType _nodeType;
	string _nodeContent;
	ASTNode* _parent;
	list<ASTNode*> _children;
	
public:
	ASTNode();
	ASTNode(NodeType, string);
	NodeType getNodeType();
	string getContent();
	void addChildren(ASTNode*);
	list<ASTNode*> getChildren();
	ASTNode* getParent();

private:
	void setParent(ASTNode*);
};
#endif
