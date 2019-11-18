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
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			int type = rand() % 12;
			Room r;
			if (type >= 0 && type <= 3) {
				r = Room();
			}
			else if (type >= 4 && type <= 6) {
				Container* _loot = _o->getContainer(validLoot.at(rand() % validLoot.size()));
				r = Room(_loot);
			}
			else if (type >= 7 && type <= 11) {
				Character* _enemy = _o->getCharacter(validEnemies.at(rand() % validEnemies.size()));
				r = Room(_enemy);
			}
			rooms[x][y] = r;
		}
	}
}

int Map::roomsCleared_()
{
	int numRooms = 0;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (rooms[x][y].clear_()) {
				numRooms++;
			}
		}
	}
	return numRooms;
}

void Map::printMap(int p_x, int p_y)
{
	char rep;
	for (int y = 0; y < 4; y++) {
		std::cout << "+---+---+---+---+" << std::endl;
		std::cout << "|   |   |   |   |" << std::endl;
		for (int x = 0; x < 4; x++) {\
			if (x == p_x && y == p_y) {
			rep = 'P';
			}
			else if (rooms[x][y].clear_()) {
				rep = 'C';
			}
			else {
				rep = '?';
			}
			std::cout << "| " << rep << " ";
		}
		std::cout << "|" << std::endl;
		std::cout << "|   |   |   |   |" << std::endl;
	}
	std::cout << "+---+---+---+---+" << std::endl;
}

int* Map::printMoveMenu(int p_x, int p_y)
{
	std::cout << "[1]Move North" << std::endl;
	std::cout << "[2]Move East" << std::endl;
	std::cout << "[3]Move South" << std::endl;
	std::cout << "[4]Move West" << std::endl;
	return new int[2]{ 1,4 };
}
