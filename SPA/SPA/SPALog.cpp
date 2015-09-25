#include "SPALog.h"

using namespace std;

SPALog::SPALog() {

}

void SPALog::log(string line) {
	ofstream myfile;
	myfile.open("test.log", ios::out | ios::app);
	myfile << line << endl;
	myfile << endl;
	myfile.close();
}

