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
	unordered_map<int, CFGNode> _nodeMap;
public:
	CFG();
	void BuildGraph(list<pair<int, string>>);
	list<int> getNext(int);
	~CFG();
private:
	int findNode(int);
	void createNode(int, list<int>);
	bool isIfStmt(string);
	bool isWhileStmt(string);
	int getFirst(list<int>);
};
#endif
