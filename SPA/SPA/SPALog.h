#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef SPALog_H
#define SPALog_H

const int ZERO_LEVEL = 0; // all error message
const int LOW_LEVEL = 1; // query content
const int MEDIUM_LEVEL = 2; // PKB content, query table
const int HIGH_LEVEL = 3; 
const int EXTREME_HIGH_LEVEL = 4;

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