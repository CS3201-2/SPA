#include <iostream>
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

	std::string sourceCode((std::istreambuf_iterator<char>(inputFile)),
		(std::istreambuf_iterator<char>()));

	Controller ctr( sourceCode );

	cout << "Loaded!" << endl;

	cin.ignore();
	cin.get();

	return 0;
}