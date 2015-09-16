#include <iostream>
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

	Controller ctr;
	ctr.intializeCode(sourceCode);

	cout << "Loaded!" << endl;
	ctr.processSource();


	cout << "Start Processing PQL" << endl;

	a.open("query.txt");

	if (a.fail()) {
		cerr << "Cannot open 'query.txt'" << endl;
		exit(1);
	}

	string line;
	while (getline(a, line))
	{
		list<string> result = ctr.processQuery(line);

		for (auto& x : result) {
			cout << x << endl;
		}
		cout << "---------------------------------" <<endl;
	}
	a.close();
	cin.ignore();
	cin.get();

	return 0;
}
