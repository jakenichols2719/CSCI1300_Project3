#pragma once
#include <vector>
#include <sstream>
#include <string>
class util
{
public:
	static std::vector<std::string> split(std::string toSplit, char delim);
	static std::string divider();
};

