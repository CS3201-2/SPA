#include "PKB.h"

using namespace std;


#ifndef DesignExtractor_H
#define DesignExtractor_H

class DesignExtractor {

private:
	void resetModifiesForProc();
	void modifyProcModifiesMap(map<int, list<int>>&, int, list<int>);
	void resetModifiesForStmt();
	void resetUsesForProc();
	void modifyProcUsesMap(map<int, list<int>>&, int, list<int>);
	void resetUsesForStmt();

public:
	DesignExtractor();
	void setCalls(list<pair<int, string>>);
	void setCallsStar();
	void setFollowsStar();
	void setParentStar();
	void resetModifies();
	void resetUses();
	void setReverseMap();
};

#endif