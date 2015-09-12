#include "QueryController.h"
#include "QueryValidator.h"
#include "QueryTree.h"
#include "QueryEvaluator.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

list<string> QueryController::processQueries(list<string> queries, PKB my_pkb) {
	//list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();
	list<string> resultList;

	for (iterQueries; iterQueries != queries.end(); iterQueries++) {
		if (!(*iterQueries).empty()) {
			QueryValidator qv;
			if (qv.isValidDecAndQuery(*iterQueries)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(my_pkb, qt);
				string result = qe.evaluate();
				resultList.push_back(result);
			}
		}
	}

	return resultList;
}
