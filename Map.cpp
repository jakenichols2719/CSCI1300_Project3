#include "Map.h"
#include "ObjectTracker.h"
#include <random>

Map::Map(int _id, std::string _name)
{
	id = _id;
	name = _name;
}

void Map::fillRooms(ObjectTracker* _o)
{
	if (validEnemies.size() == 0 || validLoot.size() == 0) {
		std::cout << "Need to add enemies or loot to the map to load!";
		return;
	}
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			int type = rand() % 6;
			Room r;
			if (type == 0) {
				r = Room();
			}
			else if (type == 1 || type == 2) {
				Container* _loot = _o->getContainer(validLoot.at(rand() % validLoot.size()));
				r = Room(_loot);
			}
			else if (type > 2 && type < 6) {
				Character* _enemy = _o->getCharacter(validEnemies.at(rand() % validEnemies.size()));
				r = Room(_enemy);
			}
			rooms[x][y] = r;
		}
	}
}
