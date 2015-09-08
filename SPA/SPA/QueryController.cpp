#include "QueryController.h"
#include "QueryValidator.h"
#include "QueryTree.h"
#include "QueryEvaluator.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

void QueryController::validateQueries(list<string> queries, PKB my_pkb) {
	//list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();

	for (iterQueries; iterQueries != queries.end(); iterQueries++) {
		if (!(*iterQueries).empty()) {
			QueryValidator qv;
			if (qv.parseString(*iterQueries)) {
				QueryTree qt = qv.getQueryTree();
				QueryEvaluator qe(my_pkb, qt);
				qe.evaluate();
			}
		}
	}

}
