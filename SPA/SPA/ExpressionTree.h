#include<string>
#include<regex>
#include<unordered_map>
#include<stack>
#include"ASTNode.h"
#include"Suffix.h"

using namespace std;

class ExpressionTree
{
	ASTNode _root;
	Suffix _suffixConvertor;
	string _suffix;

public:
	ExpressionTree(string);
	ASTNode getRoot();
private:
	void constructTree();
	void buildTree();
	
};

