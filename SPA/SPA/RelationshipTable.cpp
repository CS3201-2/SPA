#include "RelationshipTable.h"
//#include <iostream>

RelationshipTable::RelationshipTable() {
	vector<string> arg1;
	vector<string> arg2;
	
	//Modifies
	arg1 = { "stmt", "assign", "while", "prog_line", "if", "call", "procedure", 
		"string", "number" }; //
	arg2 = { "variable", "string", "all" };
	Relationship r1(2, arg1, arg2);
	relTable["modifies"] = r1;
	arg1.clear();
	arg2.clear();
	//cout << relTable.find("modifies*")->second.arg1.at(0);
	
	//Uses
	arg1 = { "stmt", "assign", "while", "prog_line", "if", "call", "procedure", 
		"string", "number" }; //
	arg2 = { "variable", "string", "all" };
	Relationship r2(2, arg1, arg2);
	relTable["uses"] = r2;
	arg1.clear();
	arg2.clear();

	//Parent
	arg1 = { "stmt", "prog_line", "while", "if", "number", "all" }; //
	arg2 = { "stmt", "assign", "prog_line", "while", "if" , "call", "number", "all" }; //
	Relationship r3(2, arg1, arg2);
	relTable["parent"] = r3;
	arg1.clear();
	arg2.clear();

	//Parent*
	arg1 = { "stmt", "prog_line", "while", "if", "number", "all" }; //
	arg2 = { "stmt", "assign", "prog_line", "while", "if", "call", "number", "all" }; //
	Relationship r4(2, arg1, arg2);
	relTable["parent*"] = r4;
	arg1.clear();
	arg2.clear();

	//Follows
	arg1 = { "stmt", "assign", "prog_line", "while", "if", "call", "number", "all" }; //
	arg2 = { "stmt", "assign", "prog_line", "while", "if", "call", "number", "all" }; //
	Relationship r5(2, arg1, arg2);
	relTable["follows"] = r5;
	arg1.clear();
	arg2.clear();

	//Follows*
	arg1 = { "stmt", "assign", "prog_line", "while", "if", "call", "number", "all" };
	arg2 = { "stmt", "assign", "prog_line", "while", "if", "call", "number", "all" };
	Relationship r6(2, arg1, arg2);
	relTable["follows*"] = r6;
	arg1.clear();
	arg2.clear();

	//Pattern-assign
	arg1 = { "variable", "string", "all" };
	arg2 = { "substring", "string", "all" };
	Relationship r7(2, arg1, arg2);
	relTable["patternAssign"] = r7;
	arg1.clear();
	arg2.clear();
	
	//Pattern-if
	arg1 = { "variable", "string", "all" };
	arg2 = { "all" };
	Relationship r8(3, arg1, arg2);
	relTable["patternIf"] = r8;
	arg1.clear();
	arg2.clear();

	//Pattern-while
	arg1 = { "variable", "string", "all" };
	arg2 = { "all" };
	Relationship r9(2, arg1, arg2);
	relTable["patternWhile"] = r9;
	arg1.clear();
	arg2.clear();

	//Next
	arg1 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "all" };
	arg2 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "all" };
	Relationship r10(2, arg1, arg2);
	relTable["next"] = r10;
	arg1.clear();
	arg2.clear();

	//Next*
	arg1 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "all" };
	arg2 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "all" };
	Relationship r11(2, arg1, arg2);
	relTable["next*"] = r11;
	arg1.clear();
	arg2.clear();

	//Calls
	arg1 = { "procedure", "string", "all" };
	arg2 = { "procedure", "string", "all" };
	Relationship r12(2, arg1, arg2);
	relTable["calls"] = r12;
	arg1.clear();
	arg2.clear();

	//Calls*
	arg1 = { "procedure", "string", "all" };
	arg2 = { "procedure", "string", "all" };
	Relationship r13(2, arg1, arg2);
	relTable["calls*"] = r13;
	arg1.clear();
	arg2.clear();

	//Affects
	arg1 = { "prog_line", "assign", "number", "all" };
	arg2 = { "prog_line", "assign", "number", "all" };
	Relationship r14(2, arg1, arg2);
	relTable["affects"] = r14;
	arg1.clear();
	arg2.clear();

	//Affects*
	arg1 = { "prog_line", "stmt", "assign", "number", "all" };
	arg2 = { "prog_line", "stmt", "assign", "number", "all" };
	Relationship r15(2, arg1, arg2);
	relTable["affects*"] = r15;
	arg1.clear();
	arg2.clear();

	//WithNumber
	arg1 = { "constant", "stmt", "assign", "while", "if", "call", "number", 
		"prog_line" };
	arg2 = { "constant", "stmt", "assign", "while", "if", "call", "number", 
		"prog_line" };
	Relationship r16(2, arg1, arg2);
	relTable["withNumber"] = r16;
	arg1.clear();
	arg2.clear();

	//WithName
	arg1 = { "procedure", "call", "variable", "string" };
	arg2 = { "procedure", "call", "variable", "string" };
	Relationship r17(2, arg1, arg2);
	relTable["withName"] = r17;
	arg1.clear();
	arg2.clear();
}

bool RelationshipTable::hasRelationship(string rel) {
	auto it = relTable.find(rel);
	if (it == relTable.end()) { //relationship not found
		return false;
	}

	return true;
}

bool RelationshipTable::isNumOfArgsEqual(string rel, int num) {
	//add assert hasRelationship
	Relationship r = relTable.find(rel)->second;
	return (r.getNumOfArgs() == num);
}

bool RelationshipTable::isArgValid(string rel, int arg, string dataType){
	switch (arg) {
	case 1: return isArg1Valid(rel, dataType);
		break;
	case 2: return isArg2Valid(rel, dataType);
		break;
	}
	return false;
}

bool RelationshipTable::isArg1Valid(string rel, string dataType) {
	//add assert hasRelationship
	Relationship r = relTable.find(rel)->second;

	vector<string> arg1 = r.getArg1();
	vector<string>::const_iterator iterArg1 = arg1.begin();

	for (iterArg1; iterArg1 != arg1.end(); iterArg1++) {
		if (dataType.compare(*iterArg1) == 0) {
			return true;
		}
	}

	return false;
}

bool RelationshipTable::isArg2Valid(string rel, string dataType) {
	//add assert hasRelationship
	Relationship r = relTable.find(rel)->second;

	vector<string> arg2 = r.getArg2();
	vector<string>::const_iterator iterArg2 = arg2.begin();

	for (iterArg2; iterArg2 != arg2.end(); iterArg2++) {
		if (dataType.compare(*iterArg2) == 0) {
			return true;
		}
	}

	return false;
}
