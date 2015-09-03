#include<string>
#include<regex>
#include <unordered_map>
#include"ASTNode.h"

using namespace std;

class ExpressionTree
{
	ASTNode _root;
public:
	ExpressionTree(string);
	ASTNode getRoot();
private:
	void constructTree(string);
	string getSuffix(string);
	void buildTree(string);
};

