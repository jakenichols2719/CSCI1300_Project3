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

	std::vector<std::string> targets; //self, other
	std::vector<std::string> stats; //hp, mp, sp, ac
	std::vector<char> directions; //+,-
	std::vector<std::string> amounts; //Any die, wep
	std::vector<int> times;
public:
	Ability(int _id, std::string _name) : id(_id), name(_name) {};
	Ability(int _id, std::string _name,
		std::vector<std::string> _targets, std::vector<std::string> _stats,
		std::vector<char> _directions, std::vector <std::string> _amounts, std::vector<int> times);

	bool addEffect(std::string _target, std::string _targetstat, char _direction, std::string _amount, int _time);
	void useAbility(Character* active, Character* other);
};

