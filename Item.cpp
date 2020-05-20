#include "Item.h"

void Item::setid(int _id)
{
	id = _id;
}

void Item::setName(std::string _name)
{
	name = _name;
}

void Item::setDesc(std::string _desc)
{
	desc = _desc;
}

void Item::setValue(int _value)
{
	value = _value;
}

std::string Item::stringRep()
{
	std::string rep = name + ": " + desc + "\n";
	rep += "Value: " + std::to_string(value);
	return rep;
}

std::string Weapon::stringRep()
{
	std::string rep = name_() + ": " + desc_() + "\n";
	rep += "Damage: " + damageDie.stringRep() + "+" + std::to_string(bonus) + "\n";
	rep += "Value: " + std::to_string(value_());
	return rep;
}

std::string Armor::stringRep()
{
	std::string rep = name_() + ": " + desc_() + "\n";
	rep += "AC Bonus: " + std::to_string(acBonus_()) + "\n";
	rep += "Value: " + std::to_string(value_());
	return rep;
	return std::string();
}
