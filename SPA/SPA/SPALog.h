#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef SPALog_H
#define SPALog_H

class SPALog
{
public:

	static SPALog* getSPALogInstance();
	static void log(string);
	void logWithLevel(int,string);
	void setLogLevel(int);

private:
	static SPALog* SPALogInstance;
	int logLevel;
};

#endif