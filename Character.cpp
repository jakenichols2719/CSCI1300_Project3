#include "Character.h"
#include <iostream>

int Character::ac_()
{
	if (equippedArmor_() != nullptr) {
		return ac + equippedArmor_()->acBonus_();
	}
	return ac;
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

bool Character::changeStat(std::string stat, int amount)
{
	if (stat == "hp") {
		hp += amount;
		if (hp > hp_max) {
			hp = hp_max;
		}
	} else if (stat == "mp") {
		mp += amount;
		if (mp > mp_max) {
			mp = mp_max;
		}
	} else if (stat == "sp") {
		sp += amount;
	} else if(stat == "ac") {
		ac += amount;
	}
	else {
		return false;
	}
	return true;
}
