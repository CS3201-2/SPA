#include<list>
#include<vector>
#include<unordered_map>
#include<stack>
#include<queue>
#include<regex>
#include<algorithm>
#include<iostream>
#include<stdexcept>
#include"SPALog.h"
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
	vector<list<int>> _nextTable;
	vector<list<int>> _beforeTable;
	unordered_map<int, CFGNode*> _nodeMap;

	list<pair<int, string>>::iterator _codeIterator;
	list<pair<int, string>> _codeLst;
	stack<pair<int,int>> _nodeInOperation;
	list<int> _statBuffer;
	int _nodeIndex;
	int _size;
public:
	CFG();
	void buildGraph(list<pair<int, string>>);
	list<int> getNextFirst(int);
	list<int> getNextSecond(int);
	bool isNextValid(int, int);
	list<int> getNextStarFirst(int);
	list<int> getNextStarSecond(int);
	bool isNextStarValid(int, int);
	void printGraph();
	void printNextTable();
	void printBeforeTable();
	void logNext();
	~CFG();
private:
	int findNode(int);
	int extractBuffer();
	int createContainerNode(int);
	int createDummyNode();
	int getType(string);
	int countBrace(string);
	list<int> traverse(int, vector<list<int>>);
	void solveCode();
	void solveNode(int,int);
	void initializeStack();
	void updateVector(int, int);
	void updateVector(int, int, vector<list<int>>&);
	void storeNextTable();
	void storeNext(int);
	bool isContainer(string);
	bool isIfStmt(string);
	bool isWhileStmt(string);
	bool isProc(string);
};
#endif
