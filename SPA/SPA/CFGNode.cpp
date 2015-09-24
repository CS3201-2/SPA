#include "CFGNode.h"



CFGNode::CFGNode()
{
}

CFGNode::CFGNode(int i, int s, int e)
{
	_index = i;
	_start = s;
	_end = e;
}


CFGNode::~CFGNode()
{
}
