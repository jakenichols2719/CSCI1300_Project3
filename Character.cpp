#include "Character.h"
#include <iostream>

void Character::regen()
{
	changeStat("mp", mp_regen, true);
	std::cout << "(" << name <<  " regenerates " << mp_regen << " mp.)" << std::endl;
}

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

int* Character::printAbilityMenu()
{
	int* bounds = new int[2];
	int n = 0;
	for (n; n < abilities.size(); n++) {
		std::cout << "[" << n+1 << "]: " << abilities.at(n)->name_() << std::endl;
	}
	bounds[0] = 1;
	bounds[1] = n + 1;
	return bounds;
}

int* Character::printInventoryMenu()
{
	std::cout << name << "'s Inventory" << std::endl;
	int* bounds = new int[2];
	int n = 0;
	for (n; n < inventory.numItems_(); n++) {
		std::cout << "[" << n + 1 << "]" << inventory.viewItem(n)->name_() << " (" << inventory.quantAt(n) << ")" << std::endl;
	}
	bounds[0] = 1;
	bounds[1] = n + 1;
	return bounds;
}

int* Character::printEquipmentMenu()
{
	std::cout << name << "'s Equipment" << std::endl;
	int* bounds = new int[2];
	int n = 0;
	for (n; n < equipment.numItems_(); n++) {
		std::cout << "[" << n + 1 << "]" << equipment.viewItem(n)->name_() << std::endl;
	}
	bounds[0] = 1;
	bounds[1] = n + 1;
	return bounds;
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
		std::cout << "You don't have that item in your inventory." << std::endl;
		std::cout << "If you're seeing this, something is VERY wrong." << std::endl;
		return -1;
	}


	std::string eqType = toEquip->type_();
	if (eqType == "wep") {
		if (equippedWeapon_() != nullptr) {
			std::cout << "You already have a weapon equipped. Unequip it first." << std::endl;
			return -2;
		}
		inventory.moveItemTo(itemid, &equipment, 1);
		std::cout << "You equipped the " << toEquip->name_() << "." << std::endl;
	}
	if (eqType == "arm") {
		if (equippedArmor_() != nullptr) {
			std::cout << "You already have armor equipped. Unequip it first." << std::endl;
			return -2;
		}
		inventory.moveItemTo(itemid, &equipment, 1);
		std::cout << "You equipped the " << toEquip->name_() << "." << std::endl;
	}
	if (eqType == "item") {
		std::cout << "You can't equip this item." << std::endl;
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
		std::cout << "You cannot unequip that, on account of you not having it. This is a problem." << std::endl;
		return -1;
	}


	bool check = equipment.moveItemTo(itemid, &inventory, 1);
	if (check) {
		std::cout << "You unequipped the " << toUnEquip->name_() << "." << std::endl;
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
	}
	else if (stat == "mp") {
		initStat = mp;
		mp += amount;
		if (mp > mp_max) {
			mp = mp_max;
		}
		if (mp < 0) {
			mp = 0;
		}
		finalStat = mp;
	}
	else if (stat == "sp") {
		initStat = sp;
		sp += amount;
		if (sp > 100) {
			sp = 100;
		}
		if (sp < 0) {
			sp = 0;
		}
		finalStat = sp;
	}
	else if(stat == "ac") {
		initStat = ac;
		ac += amount;
		if (ac < 0) {
			ac = 0;
		}
		finalStat = ac;
	}
	else if (stat == "str") {
		initStat = str;
		str += amount;
		finalStat = str;
	}
	else if (stat == "mag") {
		initStat = mag;
		mag += amount;
		finalStat = mag;
	}
	else {
		changeToken.push_back(std::to_string(0));
		return changeToken;
	}
	changeToken.push_back(std::to_string(finalStat - initStat));
	return changeToken;
}

void Character::changeStat(std::string stat, int amount, bool noreturn)
{
	if (stat == "hp") {
		hp += amount;
		if (hp > hp_max) {
			hp = hp_max;
		}
		if (hp < 0) {
			hp = 0;
		}
	}
	else if (stat == "mp") {
		mp += amount;
		if (mp > mp_max) {
			mp = mp_max;
		}
		if (mp < 0) {
			mp = 0;
		}
	}
	else if (stat == "sp") {
		sp += amount;
		if (sp > 100) {
			sp = 100;
		}
		if (sp < 0) {
			sp = 0;
		}
	}
	else if (stat == "ac") {
		ac += amount;
		if (ac < 0) {
			ac = 0;
		}
	}
	else if (stat == "str") {
		str += amount;
	}
	else if (stat == "mag") {
		mag += amount;
	}
	else {
	}
}
