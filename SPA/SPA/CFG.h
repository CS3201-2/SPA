#pragma once
#include<list>
#include<vector>
#include<unordered_map>
#include<stack>
#include"CFGNode.h"
using namespace std;
#ifndef CFG_H
#define CFG_H
class CFG
{
	vector<list<int>> _next;
	unordered_map<int, CFGNode*> _nodeMap;

	list<pair<int, string>>::iterator _codeIterator;
	list<pair<int, string>> _codeLst;
	stack<int> _nodeInOperation;
	list<int> _statBuffer;
	int _nodeIndex;
	int _currentNodeIndex;
public:
	CFG();
	void BuildGraph(list<pair<int, string>>);
	list<int> getNext(int);
	~CFG();
private:
	int findNode(int);
	void extractBuffer();
	void solveCode();
	bool isIfStmt(string);
	bool isWhileStmt(string);
};
#endif
