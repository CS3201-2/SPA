#include "CFG.h"
CFG::CFG()
{
}

void CFG::BuildGraph(list<pair<int, string>> codeLst)
{
	_next.reserve(codeLst.size());
	_statBuffer.clear();
	_codeLst = codeLst;
	_codeIterator = _codeLst.begin();
	_nodeIndex = 0;
	initializeStack();

	while (_codeIterator != _codeLst.end())
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

int CFG::extractBuffer()
{
	CFGNode* temp = NULL;
	if (!_statBuffer.empty())
	{
		temp = new CFGNode(_nodeIndex, *(_statBuffer.begin()), *(_statBuffer.end()));
		_statBuffer.clear();
		_nodeMap.insert(pair<int, CFGNode*>(_nodeIndex, temp));
		_nodeIndex++;
		return _nodeIndex-1;
	}
	else//buffer is empty
	{
		return -1;
	}
}

int CFG::createContainerNode(int index)
{
	CFGNode* temp = new CFGNode(_nodeIndex, index, index);
	_nodeMap.insert(pair<int, CFGNode*>(_nodeIndex, temp));
	_nodeIndex++;
	return _nodeIndex - 1;
}

int CFG::getType(string str)
{
	if (isIfStmt(str))
	{
		return TYPE_THEN;
	}
	else if (isWhileStmt(str))
	{
		return TYPE_WHILE;
	}
	else
	{
		throw "Something wrong here--->CFG getType";
	}
}

void CFG::solveCode()
{
	string codeContent = _codeIterator->second;
	int codeIndex = _codeIterator->first;
	if (isContainer(codeContent))
	{
		int tempNodeIndex = extractBuffer();
		if (tempNodeIndex != -1)
		{
			solveNode(tempNodeIndex, 0);
		}
		tempNodeIndex = createContainerNode(codeIndex);
		solveNode(tempNodeIndex, getType(codeContent));
	}
	else if (isProc(codeContent))
	{
		initializeStack();
		_nodeInOperation.push(Pair(-1, TYPE_PROC));
	}
	else
	{
		//here can only be noraml statement
		_statBuffer.push_back(codeIndex);
	}


	solveBrace(codeContent);
}

void CFG::solveNode(int index, int type)
{
	int topIndex = _nodeInOperation.top().first;
	int topType = _nodeInOperation.top().second;
	if (topType != TYPE_PROC)
	{
		updateVector(topIndex, index);
		if (topType == TYPE_NORMAL)
		{
			_nodeInOperation.pop();	
		}
	}
	_nodeInOperation.push(Pair(index, type));
}

void CFG::initializeStack()
{
	while (!_nodeInOperation.empty())
	{
		_nodeInOperation.pop();
	}
}

void CFG::updateVector(int position, int value)
{
	if (position < _next.size())
	{
		_next[position].push_back(value);
	}
	else
	{
		list<int> lst;
		lst.push_back(value);
		_next.push_back(lst);
	}
}

bool CFG::isContainer(string str)
{
	return isIfStmt(str) || isWhileStmt(str);
}

bool CFG::isIfStmt(string)
{
	return false;
}

bool CFG::isWhileStmt(string)
{
	return false;
}

bool CFG::isProc(string)
{
	return false;
}
