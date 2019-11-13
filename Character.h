#pragma once
#include <string>
#include <vector>
#include "Container.h"
#include "Ability.h"

class Character
{
private:
	int id;
	std::string name;
	int hp_max;
	int hp;
	int mp_max;
	int mp;
	int mp_regen;
	int sp;
	int ac;
	Container inventory;
	Container equipment;
	std::vector<Ability*> abilities;
public:
	Character(int _id, std::string _name, int _hp_max, int _mp_max, int _mp_regen, int _sp, int _ac,
		Container* _inventory, Container* _equipment) : id(_id), name(_name), hp_max(_hp_max), mp_max(_mp_max), mp_regen(_mp_regen),
		hp(_hp_max), mp(_mp_max), sp(_sp), ac(_ac), inventory(*_inventory), equipment(*_equipment) {};
	int id_() { return id; };
	std::string name_() { return name; };
	int hp_max_() { return hp_max; };
	int hp_() { return hp; };
	int mp_max_() { return mp_max; };
	int mp_() { return mp; };
	int sp_() { return sp; };
	int ac_();
	Container* inventory_() { return &inventory; };
	Container* equipment_() { return &equipment; };

	void addAbility(Ability* toAdd) { abilities.push_back(toAdd); };
	Ability* getAbility(int index);
	Item* equippedWeapon_();
	Item* equippedArmor_();
	int equip(int itemid);
	int unequip(int itemid);

	std::vector<std::string> changeStat(std::string stat, int amount);

	Character instance() { return *this; };
};

