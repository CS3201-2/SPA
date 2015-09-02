#include "Parser.h"
#include "PKB.h"
#include <string>
#include <list>

// constructor
Parser::Parser()
{

}

PKB Parser::parseSource( list<string> source ) {
	//TODO:
	// first remove blank lines
	// secondly remove extra blank spaces(trim) and numbered the stmt#

	for (list<string>::iterator itr = source.begin(); itr != source.end(); ++itr) {
		string line = *itr;
	}
	

	return pkb;
}

void Parser::trim(string& line) {
	
	// remove outer spaces
	const auto indexBegin = line.find_first_not_of(" \t");
	
	const auto indexEnd = line.find_last_not_of(" \t");
	const auto length = indexEnd - indexBegin + 1;
	line.substr(indexBegin, length);
		
	// remove inner spaces
	auto spaceStart = line.find_first_of(" \t");
	while ( spaceStart != std::string::npos)
	{
		const auto spaceEnd = line.find_first_not_of(" \t", spaceStart);
		const auto spaceLength = spaceEnd - spaceEnd;

		line.replace(spaceStart, spaceLength, " ");
		
		const auto index = spaceStart + 1;
		
		spaceStart = line.find_first_of(" \t", index );
	}
}

void Parser::removeBlankLines(list<string>& content) {
	for (list<string>::iterator itr = content.begin(); itr != content.end(); ++itr) {
		string line = *itr;
		const auto index = line.find_first_not_of(" \t");
		if (index == std::string::npos) {
			content.remove(*itr);
		}
	}
}

