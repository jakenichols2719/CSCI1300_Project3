#pragma once
#include <vector>
#include <string>
#include "Die.h"

class Character;

class Ability
{
private:
	int id;
	std::string name;
	bool rollHit;
	int mpCost;

	std::vector<std::string> targets; //self, other
	std::vector<std::string> stats; //hp, mp, sp, ac
	std::vector<char> directions; //+,-
	std::vector<std::string> amounts; //Any die, wep
	std::vector<int> times;
public:
	Ability(int _id, std::string _name, bool _rollHit) : id(_id), name(_name), rollHit(_rollHit), mpCost(0) {};
	Ability(int _id, std::string _name,
		std::vector<std::string> _targets, std::vector<std::string> _stats,
		std::vector<char> _directions, std::vector <std::string> _amounts, std::vector<int> times);

	bool addEffect(std::string _target, std::string _targetstat, char _direction, std::string _amount, int _time);
	std::vector<std::vector<std::string>> useAbility(Character* active, Character* other);

	std::string stringRep();
	std::string name_() { return name; };
	int mpCost_() { return mpCost; };
};

