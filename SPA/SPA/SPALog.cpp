#include "SPALog.h"

using namespace std;

SPALog* SPALog::SPALogInstance = NULL;

SPALog* SPALog::getSPALogInstance() {
	if (!SPALogInstance) {
		SPALogInstance = new SPALog();
	}

	return SPALogInstance;
}

void SPALog::setLogLevel(int level) {
	logLevel = level;
}

void SPALog::log(string line) {
	ofstream myfile;
	myfile.open("log/test.log", ios::out | ios::app);
	myfile << line << endl;
	myfile << endl;
	myfile.close();
}

void SPALog::logWithLevel(int level, string line) {
	if (level <= logLevel) {
		ofstream myfile;
		myfile.open("log/test.log", ios::out | ios::app);
		myfile << line << endl;
		myfile << endl;
		myfile.close();
	}
}

