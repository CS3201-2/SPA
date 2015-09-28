#include "CFG.h"
const regex REGEX_WHILE("while(([[:alpha:]])([[:alnum:]]+)*)\\{");
const regex REGEX_IF("if(([[:alpha:]])([[:alnum:]]+)*)then\\{");
const regex REGEX_PROC("procedure(([[:alpha:]])([[:alnum:]]+)*)\\{");

CFG::CFG()
{
}

void CFG::buildGraph(list<pair<int, string>> codeLst)
{
	_next.reserve(codeLst.size());
	_statBuffer.clear();
	_codeLst = codeLst;
	_codeIterator = _codeLst.begin();
	_nodeIndex = 0;
	initializeStack();

	while (_codeIterator != _codeLst.end())
	{
		cout << _codeIterator->second << endl;
		solveCode();
		_codeIterator++;
		cout << " good!" << endl;
	}

}

list<int> CFG::getNext(int i)
{
	int nodeIndex = findNode(i);
	CFGNode* node = _nodeMap.at(nodeIndex);
	if (node->contains(i + 1))
	{
		list<int> temp = list<int>();
		temp.push_back(i + 1);
		return temp;
	}
	else
	{
		return _next.at(nodeIndex);
	}
}

void CFG::printMap()
{
	cout << "size of the array is " << _next.size() << endl;
	cout << "size of the map is " << _nodeMap.size() << endl;
	for (int i = 0; i < _next.size(); i++)
	{
		list<int> temp = _next.at(i);
		CFGNode* node = _nodeMap.at(i);
		cout << i << "(" << node->getStrat() << "," << node->getEnd() << "):";
		for (list<int>::iterator it = temp.begin(); it != temp.end(); it++)
		{
			cout << " " << *it;
		}
		std::cout << endl;
	}
}

CFG::~CFG()
{
}

int CFG::findNode(int i)
{
	int begin = 0;
	int end = _nodeMap.size()-1;
	int mid = (begin + end) / 2;
	CFGNode* currentNode = NULL;
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
		int begin = _statBuffer.front();
		int end = _statBuffer.back();
		temp = new CFGNode(_nodeIndex, begin, end);
		_statBuffer.clear();
		_nodeMap.insert(std::make_pair(_nodeIndex, temp));
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
	_nodeMap.insert(std::make_pair(_nodeIndex, temp));
	_nodeIndex++;
	return _nodeIndex - 1;
}

int CFG::createDummyNode()
{
	CFGNode* temp = new CFGNode(_nodeIndex, -1, -1);
	_nodeMap.insert(std::make_pair(_nodeIndex, temp));
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

int CFG::countBrace(string str)
{
	return std::count(str.begin(), str.end(), '}');
}

void CFG::solveCode()
{
	string codeContent = _codeIterator->second;
	int codeIndex = _codeIterator->first;
	if (isContainer(codeContent))
	{
		int tempNodeIndex = extractBuffer();
		int tempContainerNodeIndex = createContainerNode(codeIndex);
		if (tempNodeIndex != -1)
		{
			solveNode(tempNodeIndex, 0);
		}
		solveNode(tempContainerNodeIndex, getType(codeContent));
	}
	else if (isProc(codeContent))
	{
		initializeStack();
		_statBuffer.clear();
		_nodeInOperation.push(std::make_pair(-1, TYPE_PROC));
	}
	else
	{
		//here can only be noraml statement
		_statBuffer.push_back(codeIndex);
		int brace = countBrace(codeContent);
		for (int i = 0; i < brace; i++)
		{
			int tempNodeIndex = extractBuffer();
			if (tempNodeIndex != -1)
			{
				solveNode(tempNodeIndex, 0);
			}

			pair<int, int> temp = _nodeInOperation.top();
			_nodeInOperation.pop();
			pair<int, int> top = _nodeInOperation.top();
			int type_temp = temp.second;
			int type_top = top.second;
			if (top.second == TYPE_WHILE)
			{
				updateVector(temp.first, top.first);
				top.second = TYPE_NORMAL;
				_nodeInOperation.pop();
				_nodeInOperation.push(top);
			}
			else if (top.second == TYPE_THEN)
			{
				top.second = TYPE_ELSE;
				_nodeInOperation.pop();
				_nodeInOperation.push(temp);
				_nodeInOperation.push(top);
				_codeIterator++;
			}
			else if (top.second == TYPE_ELSE)
			{
				_nodeInOperation.pop();
				pair<int, int> temp2 = _nodeInOperation.top();
				_nodeInOperation.pop();
				int dummy = createDummyNode();
				updateVector(temp2.first, dummy);
				updateVector(temp.first, dummy);
				_nodeInOperation.push(std::make_pair(dummy, TYPE_NORMAL));
			}
			else if (top.second == TYPE_NORMAL)
			{
				updateVector(top.first, temp.first);
				_nodeInOperation.pop();
				_nodeInOperation.push(temp);
				initializeStack();
			}
			else if (top.second == TYPE_PROC)
			{
				updateVector(temp.first, -1);
				initializeStack();
			}
			else//top.second == TYPE_PROC
			{
				throw "someting wrong here--->CFG solve code";
			}
		}
	}	
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
	_nodeInOperation.push(std::make_pair(index, type));
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
	if (position < 0)
	{

	}
	else if (position < _next.size())
	{
		if (value != _next[position].front())
		{
			_next[position].push_back(value);
		}
	}
	else
	{
		list<int> lst = list<int>();
		lst.push_back(value);
		_next.push_back(lst);
	}
}

bool CFG::isContainer(string str)
{
	return isIfStmt(str) || isWhileStmt(str);
}

bool CFG::isIfStmt(string str)
{
	bool result = regex_match(str, REGEX_IF);
	return result;
}

bool CFG::isWhileStmt(string str)
{
	return regex_match(str, REGEX_WHILE);;
}

bool CFG::isProc(string str)
{
	return regex_match(str, REGEX_PROC);
}
