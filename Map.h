#pragma once
#include <string>
#include <vector>
#include "Room.h"

class ObjectTracker;

class Map
{
private:
	int id;
	std::string name;
	std::vector<int> validEnemies;
	std::vector<int> validLoot;
	Room rooms[3][3];
public:
	Map(int _id, std::string _name);
	void addValidEnemy(int _id) { validEnemies.push_back(_id); };
	void addValidLoot(int _id) { validLoot.push_back(_id); };
	void fillRooms(ObjectTracker* _o);

	Room* getRoomAt(int x, int y) { return &rooms[x][y]; };
};

