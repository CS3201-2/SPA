#include "QueryController.h"
#include "QueryValidator.h"
#include "QueryTree.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

void QueryController::validateQueries(list<string> queries) {
	//list<bool> isValid;
	list<string>::const_iterator iterQueries = queries.begin();

	for (iterQueries; iterQueries != queries.end(); iterQueries++) {
		//isValid.push_back(parseQuery(*iterQueries));
		if (!(*iterQueries).empty()) {
			QueryValidator qv;
			if (qv.parseString(*iterQueries)) {
				//QueryTree qt = qv.getQueryTree();
				//initialize query evaluator object
				//qv.evaluate(query tree);
			}
		}
	}

}
