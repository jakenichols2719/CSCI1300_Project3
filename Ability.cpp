#include "Ability.h"
#include "Character.h"
#include <iostream>

Ability::Ability(int _id, std::string _name,
	std::vector<std::string> _targets, std::vector<std::string> _stats,
	std::vector<char> _directions, std::vector<std::string> _amounts, std::vector<int> _times)
{
	id = _id;
	name = _name;
	targets = _targets;
	stats = _stats;
	directions = _directions;
	amounts = _amounts;
	times = _times;
}

bool Ability::addEffect(std::string _target, std::string _targetstat, char _direction, std::string _amount, int _time)
{
	targets.push_back(_target);
	stats.push_back(_targetstat);
	directions.push_back(_direction);
	amounts.push_back(_amount);
	times.push_back(_time);
	return true;
}

void Ability::useAbility(Character* active, Character* other)
{
	std::cout << active->name_() + " used " + name + "!" << std::endl;

	if (rollHit) {
		int roll = Die::rollToHit(0);
		if (roll < other->ac_()) {
			std::cout << "It missed." << std::endl;
			std::cout << util::divider() << std::endl;
			return;
		}
	}

	for (unsigned n = 0; n < targets.size(); n++) {
		Character* toTarget;
		std::string printEffect = "";
		if (targets.at(n) == "self") {
			toTarget = active;
			printEffect += "You ";
		}
		else if (targets.at(n) == "other") {
			toTarget = other;
			printEffect += other->name_() + " ";
		}
		else {
			toTarget = other;
			printEffect += other->name_() + " ";
		}

		std::string statToChange = stats.at(n);
		Die changeDie;
		int changeAmount;
		if (amounts.at(n) == "wep") {
			Item* activeWep = active->equippedWeapon_();
			int bonus = 0;
			if (activeWep != nullptr) {
				changeDie = activeWep->damageDie_();
				bonus = activeWep->bonus_();
			}
			else {
				changeDie.setFromString("1d2");
			}
			changeAmount = changeDie.roll(bonus);
		}
		else {
			changeDie.setFromString(amounts.at(n));
			changeAmount = changeDie.roll(0);
		}

		if (directions.at(n) == '-') {
			changeAmount -= 2 * changeAmount;
			printEffect += "lost ";
		}
		else {
			printEffect += "gained ";
		}
		printEffect += std::to_string(abs(changeAmount)) + " " + statToChange + ".";

		std::cout << printEffect << std::endl;
		//change stat: will eventually need reworking to factor in time
		toTarget->changeStat(statToChange, changeAmount);
	}
	std::cout << util::divider() << std::endl;
}

std::string Ability::stringRep()
{
	std::string rep = name;
	for (int n = 0; n < targets.size(); n++) {
		rep += "\n" + std::string(1, directions.at(n));
		if (amounts.at(n) == "wep") {
			rep += "weapon damage";
		}
		else {
			rep += amounts.at(n);
		}
		rep += " to " + targets.at(n) + " " + stats.at(n);
		if (times.at(n) != -1) {
			rep += " for " + std::to_string(times.at(n)) + " turn(s)";
		}
	}
	if (rollHit) {
		rep += "\nMust roll to hit";
	}
	return rep;
}
