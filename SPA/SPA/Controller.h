#include <string>
#include <list>

using namespace std;

#ifndef Controller_H
#define Controller_H

class Controller
{
public:
	Controller( list<string> );

	void display( void );

private:
	list<string> source;
};

#endif