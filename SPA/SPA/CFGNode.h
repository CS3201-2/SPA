using namespace std;
#ifndef CFGNode_H
#define CFGNode_H
class CFGNode
{
	int _index;
	int _start;
	int _end;
public:
	CFGNode();
	CFGNode(int, int, int);
	int getStart();
	int getEnd();
	bool contains(int);
	bool isLargerThan(int);
	bool isSmallerThan(int);
	~CFGNode();
};
#endif
