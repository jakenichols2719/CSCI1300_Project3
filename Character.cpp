#include "Character.h"
#include <iostream>

int Character::ac_()
{
	if (equippedArmor_() != nullptr) {
		return ac + equippedArmor_()->acBonus_();
	}
	return ac;
}

Ability* Character::getAbility(int index)
{
	if (index < 0 || index >= abilities.size()) {
		return nullptr;
	}
	return abilities.at(index);
}

Item* Character::equippedWeapon_()
{
	Item* wep = nullptr;
	for (int n = 0; n < equipment.numItems_(); n++) {
		Item* check = equipment.viewItem(n);
		if (check != nullptr && check->type_() == "wep") {
			wep = check;
		}
	}
	return wep;
}

Item* Character::equippedArmor_()
{
	Item* arm = nullptr;
	for (int n = 0; n < equipment.numItems_(); n++) {
		Item* check = equipment.viewItem(n);
		if (check != nullptr && check->type_() == "arm") {
			arm = check;
		}
	}
	return arm;
}

int Character::equip(int itemid)
{
	Item* toEquip = nullptr;
	for (int n = 0; n < inventory.numItems_(); n++) {
		Item* toCheck = inventory.viewItem(n);
		if (toCheck->id_() == itemid) {
			toEquip = toCheck;
		}
	}
	if (toEquip == nullptr) {
		return -1;
	}


	std::string eqType = toEquip->type_();
	if (eqType == "wep") {
		if (equippedWeapon_() != nullptr) {
			return -2;
		}
		inventory.moveItemTo(itemid, &equipment);
	}
	if (eqType == "arm") {
		if (equippedArmor_() != nullptr) {
			return -2;
		}
		inventory.moveItemTo(itemid, &equipment);
	}
	return 0;
}

int Character::unequip(int itemid)
{
	Item* toUnEquip = nullptr;
	for (int n = 0; n < equipment.numItems_(); n++) {
		Item* toCheck = equipment.viewItem(n);
		if (toCheck->id_() == itemid) {
			toUnEquip = toCheck;
		}
	}
	if (toUnEquip == nullptr) {
		return -1;
	}


	bool check = equipment.moveItemTo(itemid, &inventory);
	if (check) {
		return 0;
	}
	else {
		return -2;
	}
}

std::vector<std::string> Character::changeStat(std::string stat, int amount)
{
	int initStat;
	int finalStat;
	std::vector<std::string> changeToken;
	changeToken.push_back(stat);

	if (stat == "hp") {
		initStat = hp;
		hp += amount;
		if (hp > hp_max) {
			hp = hp_max;
		}
		if (hp < 0) {
			hp = 0;
		}
		finalStat = hp;
	} else if (stat == "mp") {
		initStat = mp;
		mp += amount;
		if (mp > mp_max) {
			mp = mp_max;
		}
		if (mp < 0) {
			mp = 0;
		}
		finalStat = mp;
	} else if (stat == "sp") {
		initStat = sp;
		sp += amount;
		if (sp > 100) {
			sp = 100;
		}
		if (sp < 0) {
			sp = 0;
		}
		finalStat = sp;
	} else if(stat == "ac") {
		initStat = ac;
		ac += amount;
		if (ac < 0) {
			ac = 0;
		}
		finalStat = ac;
	}
	else {
		changeToken.push_back(std::to_string(0));
		return changeToken;
	}
	changeToken.push_back(std::to_string(finalStat - initStat));
	return changeToken;
}
