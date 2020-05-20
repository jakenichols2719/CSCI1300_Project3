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

	Die _mpCostDie;
	int _mpCost = 0;
	for (int n = 0; n < stats.size(); n++) {
		if (stats.at(n) == "mp" && targets.at(n) == "self") {
			_mpCostDie = Die(amounts.at(n));
			_mpCost = _mpCostDie.roll(0);
		}
	}
	mpCost = _mpCost;
}

bool Ability::addEffect(std::string _target, std::string _targetstat, char _direction, std::string _amount, int _time, std::string _bonus)
{
	targets.push_back(_target);
	stats.push_back(_targetstat);
	directions.push_back(_direction);
	amounts.push_back(_amount);
	times.push_back(_time);
	bonuses.push_back(_bonus);

	Die _mpCostDie;
	int _mpCost = 0;
	for (int n = 0; n < stats.size(); n++) {
		if (stats.at(n) == "mp" && targets.at(n) == "self") {
			_mpCostDie = Die(amounts.at(n));
			_mpCost = _mpCostDie.roll(0);
		}
	}
	mpCost = _mpCost;

	return true;
}

std::vector<std::vector<std::string>> Ability::useAbility(Character* active, Character* other)
{
	std::vector<std::vector<std::string>> changeTokens; //list of change tokens for each stat change
	std::cout << active->name_() + " used " + name + "!" << std::endl;

	if (active->mp_() < mpCost) {
		std::cout << "Not enough MP." << std::endl;
		std::cout << util::divider() << std::endl;
		return changeTokens;
	}

	if (rollHit) {
		int roll = Die::rollToHit(0);
		if (roll < other->ac_()) {
			std::cout << "It missed." << std::endl;
			active->changeStat("mp", -mpCost, true);
			if (mpCost != 0) {
				std::cout << active->name_() << " lost " << mpCost << " mp." << std::endl;
			}
			std::cout << util::divider() << std::endl;
			return changeTokens;
		}
	}

	for (unsigned n = 0; n < targets.size(); n++) {
		std::vector<std::string> changeToken; //individual change token
		Character* toTarget;
		std::string printEffect = "";
		//set effect change target
		if (targets.at(n) == "self") {
			toTarget = active;
			printEffect += active->name_() + " ";
		}
		else if (targets.at(n) == "other") {
			toTarget = other;
			printEffect += other->name_() + " ";
		}
		else {
			toTarget = other;
			printEffect += other->name_() + " ";
		}

		//set stat to change
		std::string statToChange = stats.at(n);

		Die changeDie;
		int changeAmount;
		//set change amount
		if (amounts.at(n) == "wep") {
			Item* activeWep = active->equippedWeapon_();
			int bonus = 0;
			if (activeWep != nullptr) {
				changeDie = activeWep->damageDie_();
				if (bonuses.at(n) == "none") {
					bonus = activeWep->bonus_();
				}
				if (bonuses.at(n) == "str") {
					bonus = active->str_() + activeWep->bonus_();
				}
				if (bonuses.at(n) == "mag") {
					bonus = active->mag_() + activeWep->bonus_();
				}
			}
			else {
				changeDie.setFromString("1d2");
			}
			changeAmount = changeDie.roll(bonus);
			if (changeAmount < 0) {
				changeAmount = 0;
			}
		}
		else {
			changeDie.setFromString(amounts.at(n));
			int bonus = 0;
			if (bonuses.at(n) == "none") {
				bonus = 0;
			}
			if (bonuses.at(n) == "str") {
				bonus = active->str_();
			}
			if (bonuses.at(n) == "mag") {
				bonus = active->mag_();
			}
			changeAmount = changeDie.roll(bonus);
		}

		//set direction
		if (directions.at(n) == '-') {
			changeAmount -= 2 * changeAmount;
			printEffect += "lost ";
		}
		else {
			printEffect += "gained ";
		}
		printEffect += std::to_string(abs(changeAmount)) + " " + statToChange;
		if (times.at(n) == -1) {
			printEffect += ".";
		}
		else {
			printEffect += " for " + std::to_string(times.at(n)) + " turns.";
		}

		std::cout << printEffect << std::endl;
		changeToken = toTarget->changeStat(statToChange, changeAmount);
		changeToken.push_back(std::to_string(times.at(n)));
		changeToken.push_back(targets.at(n));
		changeTokens.push_back(changeToken);
	}
	std::cout << util::divider() << std::endl;
	return changeTokens;
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
		if (bonuses.at(n) != "none") {
			rep += ", modified by " + bonuses.at(n);
		}
	}
	if (rollHit) {
		rep += "\nMust roll to hit";
	}
	return rep;
}
