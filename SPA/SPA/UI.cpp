/*#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Controller.h"

using namespace std;

int main(void) {
	ifstream inputFile;
	ifstream a;

	cout << "Welcome to Team Sigma's SPA project!" << endl;
	
	cout << "Loading SOURCE CODE..." << endl;

	inputFile.open( "prototype_test.txt" );
	
	if ( inputFile.fail() ) {
		cerr << "Cannot open 'prototype_test.txt'" << endl;
	    exit(1);
	}

	std::string sourceCode((std::istreambuf_iterator<char>(inputFile)),
		(std::istreambuf_iterator<char>()));

	Controller ctr( sourceCode );

	cout << "Loaded!" << endl;
	ctr.processSource();


	cout << "Start Processing PQL" << endl;

	a.open("query.txt");

	if (a.fail()) {
		cerr << "Cannot open 'query.txt'" << endl;
		exit(1);
	}

	std::string sourceQuery((std::istreambuf_iterator<char>(a)),
		(std::istreambuf_iterator<char>()));

	ctr.processQuery(sourceQuery);

	cin.ignore();
	cin.get();

	return 0;
}*/
