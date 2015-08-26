#include "Controller.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

typedef list<string> StringList;
Controller::Controller( list<string> sourceCode) {
	source = sourceCode;
}

void Controller::display() {
	for (StringList::const_iterator itr = source.begin(); itr != source.end(); ++itr)
         cout << *itr << endl;
}

