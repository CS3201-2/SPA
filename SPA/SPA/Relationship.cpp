#include "Relationship.h"

Relationship::Relationship()
{
}

Relationship::Relationship(int num, vector<string> argument1, vector<string> argument2) {
	numOfArgs = num;
	arg1 = argument1;
	arg2 = argument2;
}

/*void Relationship::setNumOfArgs(int num) {
}

void Relationship::setArg1(vector<string> arg) {
}

void Relationship::setArg2(vector<string> arg) {
}*/

int Relationship::getNumOfArgs() {
	return numOfArgs;
}

vector<string> Relationship::getArg1() {
	return arg1;
}

vector<string> Relationship::getArg2() {
	return arg2;
}

/*bool Relationship::isNumOfArgsEqual(int num) {
	return false;
}

bool Relationship::isArg1Valid() {
	return false;
}

bool Relationship::isArg2Valid() {
	return false;
}*/
