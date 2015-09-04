#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "Relationship.h"

using namespace std;

class RelationshipTable {
private:
	unordered_map<string, Relationship> relTable;
public:
	RelationshipTable();
	bool hasRelationship(string rel);
	bool isNumOfArgsEqual(string rel, int num);
	bool isArg1Valid(string rel, string dataType);
	bool isArg2Valid(string rel, string dataType);
};
