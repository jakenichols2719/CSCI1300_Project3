#pragma once
#include <string>
#include <sstream>
#include <vector>
class Die
{
private:
	int quantity;
	int maxRoll;
public:
	Die();
	Die(std::string fromString);
	void setFromString(std::string fromString);
	int roll(int bonus);
	static int rollToHit(int bonus);
	std::string stringRep();
};