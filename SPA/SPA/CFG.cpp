#include "CFG.h"
const string EXCEPT_BOUND = "Exception: index out of bound!";

CFG::CFG()
{
}

int CFG::getFirstStatement(int ProcNo)
{
	return _procTable[ProcNo];
}

int CFG::getProc(int statementNo)
{
	return _procMap.at(statementNo);
}

void CFG::buildGraph(list<Statement> codeLst)
{
	_next.resize(codeLst.size(), list<int>());
	_typeTable.resize(codeLst.size(), assignmentStmt);
	_statBuffer.clear();
	_codeLst = codeLst;
	_codeIterator = _codeLst.begin();
	_nodeIndex = 0;
	_size = 0;
	_currentProc = -1;
	_procMap.clear();
	_procTable.clear();
	initializeStack();

	while (_codeIterator != _codeLst.end())
	{
		solveCode();
		_codeIterator++;
	}
	storeNextTable();
}

list<int> CFG::getNextFirst(int i)
{
	if (i <= _size && i >= 1)
	{
		return _beforeTable[i];
	}
	else
	{
		throw invalid_argument(EXCEPT_BOUND);
	}
}

list<int> CFG::getNextSecond(int i)
{
	if (i <= _size && i >= 1)
	{
		return _nextTable[i];
	}
	else
	{
		throw invalid_argument(EXCEPT_BOUND);
	}
}

bool CFG::isNextValid(int i, int j)
{
	if (i <= _size && i >= 1 && 
		j <= _size && j >= 1)
	{
		list<int> temp = getNextSecond(i);
		for (auto& x : temp)
		{
			if (x == j)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		throw invalid_argument(EXCEPT_BOUND);
	}
}

list<int> CFG::getNextStarFirst(int i)
{
	if (i <= _size && i >= 1)
	{
		return traverse(i, _beforeTable);
	}
	else
	{
		throw invalid_argument(EXCEPT_BOUND);
	}
}

list<int> CFG::getNextStarSecond(int i)
{
	if (i <= _size && i >= 1)
	{
		return traverse(i, _nextTable);
	}
	else
	{
		throw invalid_argument(EXCEPT_BOUND);
	}
}

bool CFG::isNextStarValid(int i, int j)
{
	if (!(i <= _size && i >= 1))
	{
		throw invalid_argument(EXCEPT_BOUND);
	}
	vector<bool> visit;
	queue<int> toVisit;
	list<int> temp = _nextTable[i];
	visit.resize(_size+1, false);
	for (auto& x : temp)
	{
		toVisit.push(x);
		visit[x] = true;
	}
	while (!toVisit.empty())
	{
		int tempIndex = toVisit.front();
		toVisit.pop();
		if (tempIndex == j)
		{
			return true;
		}
		temp = _nextTable[tempIndex];
		for (auto& x : temp)
		{
			if (!visit[x]) {
				toVisit.push(x);
				visit[x] = true;
			}
		}
	}
	return false;
}

StatementType CFG::getType(int i)
{
	return _typeTable[i];
}

void CFG::printGraph()
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

void CFG::printNextTable()
{
	for (int j = 0; j <= _size;j++)
	{
		list<int> x = _nextTable[j];
		if (j != 0)
		{
			cout << j << ":";
			for (auto& y : x)
			{
				cout << y << " ";
			}
			cout << endl;
		}
		else
		{
			
		}
	}
}

void CFG::printBeforeTable()
{
	for (int j = 0; j <= _size; j++)
	{
		list<int> x = _beforeTable[j];
		if (j != 0)
		{
			cout << j << ":";
			for (auto& y : x)
			{
				cout << y << " ";
			}
			cout << endl;
		}
		else
		{

		}
	}
}

void CFG::logNext()
{
	string str = "next table\n";
	for (int j = 0; j <= _size; j++)
	{
		list<int> x = _nextTable[j];
		if (j != 0)
		{
			str += (to_string(j) + ":");
			for (auto& y : x)
			{
				str += (to_string(y) + " ");
			}
			str += "\n";
		}
		else
		{

		}
	}
	SPALog::log(str);
}

CFG::~CFG()
{
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

int CFG::getType(Statement s)
{
	if (isIfStmt(s))
	{
		return TYPE_THEN;
	}
	else if (isWhileStmt(s))
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

list<int> CFG::traverse(int index, vector<list<int>> table)
{
	list<int> buffer;
	vector<bool> visit;
	queue<int> toVisit;
	list<int> temp = table[index];
	visit.resize(_size+1, false);
	for (auto& x : temp)
	{
		toVisit.push(x);
		visit[x] = true;
	}
	while (!toVisit.empty())
	{
		int tempIndex = toVisit.front();
		toVisit.pop();
		buffer.push_back(tempIndex);
		temp = table[tempIndex];
		for (auto& x : temp)
		{
			if (!visit[x]) {
				toVisit.push(x);
				visit[x] = true;
			}
		}
	}
	return buffer;
}

void CFG::solveCode()
{
	string codeContent = _codeIterator->getContent();
	int codeIndex = _codeIterator->getNumber();
	if (codeIndex != -1)
	{ 
		_procMap.insert(make_pair(codeIndex, _currentProc));
		_typeTable[codeIndex] = _codeIterator->getType();
		if (!_procFirstStatementSet)
		{
			_procTable.push_back(codeIndex);
			_procFirstStatementSet = true;
		}
	}
	if (isContainer(*_codeIterator))
	{
		int tempNodeIndex = extractBuffer();
		int tempContainerNodeIndex = createContainerNode(codeIndex);
		if (tempNodeIndex != -1)
		{
			solveNode(tempNodeIndex, 0);
		}
		solveNode(tempContainerNodeIndex, getType(*_codeIterator));
	}
	else if (isProc(*_codeIterator))
	{
		_currentProc++;
		_procFirstStatementSet = false;
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
	else
	{
		if (_next[position].empty() || value != _next[position].front())
		{
			_next[position].push_back(value);
		}
	}
}

void CFG::updateVector(int position, int value, vector<list<int>>& temp)
{
	if (position < 0)
	{
		throw "cannot be like this lah!";
	}
	else
	{
		temp[position].push_back(value);
	}
}

void CFG::storeNextTable()
{
	list<int> empty;
	_nextTable.resize(_codeLst.size()+1, empty);
	_beforeTable.resize(_codeLst.size()+1, empty);
	for (int i = 0; i < _next.size(); i++)
	{
		try {
			storeNext(i);
		}
		catch (exception e)
		{
			break;
		}
	}
}

void CFG::storeNext(int index)
{
	CFGNode* node = _nodeMap.at(index);
	int begin = node->getStrat();
	int end = node->getEnd();
	int i = 0;
	list<int> temp;
	list<int> buffer;
	if (begin == -1)
	{
		return;
	}
	for (i = begin; i < end; i++)
	{
		temp.push_back(i + 1);
		_nextTable[i] = temp;
		_size++;
		updateVector(i+1, i, _beforeTable);
		temp.clear();
	}
	temp = _next[index];
	for (auto& x: temp)
	{
		while (x != -1)
		{
			CFGNode* tempNode = _nodeMap.at(x);
			if (tempNode->getStrat() != -1)
			{
				buffer.push_back(tempNode->getStrat());
				updateVector(tempNode->getStrat(), end, _beforeTable);
				break;
			}
			else
			{
				x = _next[x].front();
			}
		}
	}
	_nextTable[end] = buffer;
	_size++;
}

bool CFG::isContainer(Statement s)
{
	return s.getType()==ifStmt || s.getType()==whileStmt;
}

bool CFG::isIfStmt(Statement s)
{
	return s.getType() == ifStmt;
}

bool CFG::isWhileStmt(Statement s)
{
	return s.getType() == whileStmt;
}

bool CFG::isProc(Statement s)
{
	return s.getType() == procDeclarationStmt;
}

