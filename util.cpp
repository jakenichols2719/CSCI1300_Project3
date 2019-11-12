#include "util.h"

std::vector<std::string> util::split(std::string toSplit, char delim)
{
		std::vector<std::string> tokens;
		std::stringstream s(toSplit);
		std::string tempData;
		while (std::getline(s, tempData, delim)) {
			tokens.push_back(tempData);
		}
		return tokens;
}

std::string util::divider()
{
	return "#==================================================#";
}
