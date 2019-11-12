#include "Die.h"
#include "util.h"

Die::Die()
{
	quantity = 0;
	maxRoll = 0;
}

Die::Die(std::string fromString)
{
	setFromString(fromString);
}

void Die::setFromString(std::string fromString)
{
	std::vector<std::string> splitInput = util::split(fromString, 'd');
	quantity = std::stoi(splitInput.at(0));
	maxRoll = std::stoi(splitInput.at(1));
}

int Die::roll(int bonus)
{
	int totalRoll = 0;
	for (int n = 0; n < quantity; n++) {
		totalRoll += rand() % maxRoll + 1;
	}
	totalRoll += bonus;
	return totalRoll;
}

int Die::rollToHit(int bonus)
{
	return rand() % 20 + 1 + bonus;
}

std::string Die::stringRep()
{
	std::string rep = std::to_string(quantity) + "d" + std::to_string(maxRoll);
	return rep;
}

