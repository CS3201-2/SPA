#include "CFG.h"
CFG::CFG()
{
}

void CFG::BuildGraph(list<pair<int, string>> codeLst)
{
	_next.reserve(codeLst.size());
	_statBuffer.clear();
	_codeLst = codeLst;
	_codeIterator = _codeLst.begin;
	_nodeIndex = 0;
	_currentNodeIndex = 0;

	while (_codeIterator != _codeLst.end)
	{
		solveCode();
	}

}

list<int> CFG::getNext(int i)
{
	int nodeIndex = findNode(i);
	CFGNode* node = _nodeMap.at(nodeIndex);
	if (node->contains(i + 1))
	{
		list<int> temp;
		temp.push_back(i + 1);
		return temp;
	}
	else
	{
		return _next.at(nodeIndex);
	}
}

CFG::~CFG()
{
}

int CFG::findNode(int i)
{
	size_t begin = 0;
	size_t end = _nodeMap.size()-1;
	size_t mid = (begin + end) / 2;
	CFGNode* currentNode;
	if (i < begin || i > end)//i is illegal
	{
		return -1;
	}
	while (begin < end)
	{
		currentNode = _nodeMap.at(mid);
		if (currentNode->contains(i))//find the node
		{
			return mid;
		}
		else if (currentNode->isLargerThan(i))//go to left part
		{
			end = mid - 1;
			mid = (begin + end) / 2;
		}
		else if (currentNode->isSmallerThan(i))//go to right part
		{
			begin = mid + 1;
			mid = (begin + end) / 2;
		}
		else//cannot be like this
		{
			throw "Something wrong here--->CFG findNode() 1";
		}
	}
	currentNode = _nodeMap.at(mid);
	if (currentNode->contains(i))//find the node
	{
		return mid;
	}
	else
	{
		throw "Something wrong here--->CFG findNode() 2";
	}
}

void CFG::extractBuffer()
{
	if (!_statBuffer.empty())
	{
		CFGNode* temp = new CFGNode(_nodeIndex, *(_statBuffer.begin), *(_statBuffer.end));
		_nodeMap.insert(pair<int, CFGNode*>(_nodeIndex, temp));
		_currentNodeIndex = _nodeIndex;
	}
}

void CFG::solveCode()
{
	string codeContent = _codeIterator->second;
	int codeIndex = _codeIterator->first;
	if (isIfStmt(codeContent))
	{

	}
	else if (isWhileStmt(codeContent))
	{

	}
	else
	{
		_statBuffer.push_back(codeIndex);
	}
}
