#pragma once
#include <string>
#include "Die.h"


class Item
{
private:
	std::string type;
	int id;
	std::string name;
	std::string desc;
	int value;
public:
	Item() : type("item"), id(0), name(""), desc(""), value(0) {};
	Item(std::string _type, int _id, std::string _name, std::string _desc, int _value) : type(_type), id(_id), name(_name), desc(_desc), value(_value) {};
	std::string type_() { return type; };
	int id_() { return id; };
	std::string name_() { return name; };
	std::string desc_() { return desc; };
	int value_() { return value; };
	void setid(int _id);
	void setName(std::string _name);
	void setDesc(std::string _desc);
	void setValue(int _value);

	virtual std::string stringRep();

	virtual Die damageDie_() { return Die(); };
	virtual int bonus_() { return 0; };
	virtual int acBonus_() { return 0; };
};

class Weapon : public Item
{
private:
	Die damageDie;
	int bonus;
public:
	Weapon() : Item(), damageDie("0d0"), bonus(0) {};
	Weapon(std::string _type, int _id, std::string _name, std::string _desc, int _value,
		Die _damageDie, int _bonus) : Item(_type, _id, _name, _desc, _value), damageDie(_damageDie), bonus(_bonus) {};
	Die damageDie_() { return damageDie; };
	int bonus_() { return bonus; };

	std::string stringRep();
};

class Armor : public Item
{
private:
	int acBonus;
public:
	Armor() : Item(), acBonus(0) {};
	Armor(std::string _type, int _id, std::string _name, std::string _desc, int _value,
		int _acBonus) : Item(_type, _id, _name, _desc, _value), acBonus(_acBonus) {};
	int acBonus_() { return acBonus; };

	std::string stringRep();
};

