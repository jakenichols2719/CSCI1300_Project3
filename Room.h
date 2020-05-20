#pragma once
#include <string>
#include "Character.h"
#include "Container.h"

class Room
{
private:
	std::string type; //empty, fight, loot
	Character* enemy;
	Container* loot;
	bool clear = false;
public:
	Room() : type("empty"), enemy(nullptr), loot(nullptr) {};
	Room(Character* _enemy) : type("fight"), enemy(_enemy), loot(nullptr) {};
	Room(Container* _loot) : type("loot"), loot(_loot), enemy(nullptr) {};

	std::string type_() { return type; };
	Character enemy_() { return enemy->instance(); };
	Container loot_() { return loot->instance(); };
	bool clear_() { return clear; };

	void setClear() { clear = true; };
};

