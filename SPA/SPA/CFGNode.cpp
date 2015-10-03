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

int CFGNode::getStrat()
{
	return _start;
}

int CFGNode::getEnd()
{
	return _end;
}

bool CFGNode::contains(int i)
{
	return i>=_start && i<=_end;
}

bool CFGNode::isLargerThan(int i)
{
	return i<_start;
}

bool CFGNode::isSmallerThan(int i)
{
	return i>_end;
}


CFGNode::~CFGNode()
{
}
