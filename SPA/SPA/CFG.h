#include<list>
#include<vector>
#include<unordered_map>
#include<stack>
#include<queue>
#include<regex>
#include<algorithm>
#include<iostream>
#include<stdexcept>
#include<sstream>
#include"SPALog.h"
#include"CFGNode.h"
#include"Statement.h"
#include"StatementType.h"
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

	vector<StatementType> _typeTable;
	unordered_map<int, int> _procMap;
	vector<int> _procTable;
	unordered_map<int, CFGNode*> _nodeMap;

	list<Statement>::iterator _codeIterator;
	list<Statement> _codeLst;
	stack<pair<int,int>> _nodeInOperation;
	list<int> _statBuffer;
	SPALog _log;
	int _nodeIndex;
	int _size;
	int _currentProc;
	bool _procFirstStatementSet;
public:
	CFG();
	int getFirstStatement(int);
	int getProc(int);
	StatementType getType(int);

	void buildGraph(list<Statement>);

	list<int> getNextFirst(int);
	list<int> getNextSecond(int);
	bool isNextValid(int, int);

	list<int> getNextStarFirst(int);
	list<int> getNextStarSecond(int);
	bool isNextStarValid(int, int);
	
	void logNext();
	~CFG();
private:
	int extractBuffer();
	int createContainerNode(int);
	int createDummyNode();
	int getType(Statement);
	int countBrace(string);
	list<int> traverse(int, vector<list<int>>);
	void solveCode();
	void solveNode(int,int);
	void initializeStack();
	void updateVector(int, int);
	void updateVector(int, int, vector<list<int>>&);
	void storeNextTable();
	void storeNext(int);
	bool isContainer(Statement);
	bool isIfStmt(Statement);
	bool isWhileStmt(Statement);
	bool isProc(Statement);
};
#endif
