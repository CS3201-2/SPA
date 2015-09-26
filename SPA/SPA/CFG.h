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
	const int TYPE_PROC = -1;
	const int TYPE_NORMAL = 0;
	const int TYPE_WHILE = 1;
	const int TYPE_THEN = 2;
	const int TYPE_ELSE = 3;
	vector<list<int>> _next;
	unordered_map<int, CFGNode*> _nodeMap;

	list<pair<int, string>>::iterator _codeIterator;
	list<pair<int, string>> _codeLst;
	stack<pair<int,int>> _nodeInOperation;
	list<int> _statBuffer;
	int _nodeIndex;
public:
	CFG();
	void BuildGraph(list<pair<int, string>>);
	list<int> getNext(int);
	~CFG();
private:
	int findNode(int);
	int extractBuffer();
	int createContainerNode(int);
	void solveCode();
	void solveNode(int,int);
	void initializeStack();
	void updateVector(int, int);
	bool isContainer(string);
	bool isIfStmt(string);
	bool isWhileStmt(string);
	bool isProc(string);
	template <typename T>
	Pair<T,T> Pair(T, T)
	{
		return pair<T, T>(T, T);
	}
};
#endif
