/*#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Controller.h"

using namespace std;

int main(void) {
	ifstream inputFile;

	cout << "Welcome to Team Sigma's SPA project!" << endl;
	
	cout << "Loading SOURCE CODE..." << endl;

	inputFile.open( "prototype_test.txt" );
	
	if ( inputFile.fail() ) {
		cerr << "Cannot open 'prototype_test.txt'" << endl;
	    exit(1);
	}

	string line;
	list<string> sourceCode;

	while ( getline( inputFile, line ) ) {
        sourceCode.push_back( line );
	}

	Controller ctr( sourceCode );

	cout << "Loaded!" << endl;

	ctr.display();

	cin.ignore();
	cin.get();

	return 0;
}*/