#include "Ability.h"
#include "Character.h"

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
	for (unsigned n = 0; n < targets.size(); n++) {
		Character* toTarget;
		if (targets.at(n) == "self") {
			toTarget = active;
		}
		else if (targets.at(n) == "other") {
			toTarget = other;
		}
		else {
			toTarget = other;
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
		}

		toTarget->changeStat(statToChange, changeAmount);
	}
}
