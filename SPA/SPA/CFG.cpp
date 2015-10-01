/*#include "CFG.h"
CFG::CFG()
{
}

void CFG::BuildGraph(list<pair<int, string>> codeLst)
{
	stack<int> nodeInOperation;
	int nodeIndex = 0;
	list<int> statBuffer;
	int stmtIndex = 0;
	string stmtString = "";
	list<pair<int, string>>::iterator it;
	it = codeLst.begin;
	while (it != codeLst.end)
	{
		stmtIndex = it->first;
		stmtString = it->second;
		if (isIfStmt(stmtString))
		{
			createNode(nodeIndex, statBuffer);
			nodeIndex++;
			statBuffer.clear();
			createNode(nodeIndex, stmtIndex);
			nodeInOperation.push(nodeIndex);
			nodeInOperation.push(nodeIndex);
		}
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

void CFG::createNode(int index, list<int> buffer)
{
	CFGNode* temp = new CFGNode(index, *buffer.begin, *buffer.end);
	_nodeMap.insert(pair<int, CFGNode*>(index, temp));
}

void CFG::createNode(int index, int stmtIndex)
{
	CFGNode* temp = new CFGNode(index, stmtIndex, stmtIndex);
	_nodeMap.insert(pair<int, CFGNode*>(index, temp));
}*/
