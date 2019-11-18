// proj3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "ObjectTracker.h"
#include "Character.h"
#include "Ability.h"
#include "Map.h"

void run(ObjectTracker* tracker);
void fight(Character*, Character*);
int getInput(int* bounds);

int main()
{
	srand(time(NULL));
	
	ObjectTracker o;
	o.readItems("items.txt");
	o.readContainers("containers.txt");
	o.readAbilities("abilities.txt");
	o.readCharacters("characters.txt");

	run(&o);
}

void run(ObjectTracker* tracker) {
	//select character
	Character player = tracker->getCharacter(0)->instance();
	//set up map (temp)
	Map floor(0, "Cave");
	floor.addValidEnemy(2);
	floor.addValidLoot(3);
	floor.fillRooms(tracker);

	//loop variables
	bool running = true;
	int p_x = 0;
	int p_y = 0;
	floor.clearRoomAt(0, 0);
	while (running) {
		floor.printMap(p_x, p_y);
		std::cout << "Choose an action:" << std::endl;
		int bounds[]{ 1,3 };
		std::cout << "[1]Move" << std::endl;
		std::cout << "[2]View Equipment" << std::endl;
		std::cout << "[3]View Inventory" << std::endl;
		int input = getInput(bounds);
		std::cout << util::divider() << std::endl;
		//movement
		if (input == 1) {
			while (true) {
				int* bounds = floor.printMoveMenu(p_x, p_y);
				int movedir = getInput(bounds);
				if (movedir == 1) {
					if (p_y > 0) {
						p_y--;
						break;
					}
					else {
						std::cout << "You can't move north from here." << std::endl;
					}
				}
				if (movedir == 2) {
					if (p_x < 3) {
						p_x++;
						break;
					}
					else {
						std::cout << "You can't move east from here." << std::endl;
					}
				}
				if (movedir == 3) {
					if (p_y < 3) {
						p_y++;
						break;
					}
					else {
						std::cout << "You can't move south from here." << std::endl;
					}
				}
				if (movedir == 4) {
					if (p_x > 0) {
						p_x--;
						break;
					}
					else {
						std::cout << "You can't move west from here." << std::endl;
					}
				}
			}
			std::cout << util::divider() << std::endl;
			Room* currentRoom = floor.getRoomAt(p_x, p_y);
			if (currentRoom->clear_()) {
				std::cout << "You've been here before... The room is empty." << std::endl;
			}
			if (currentRoom->clear_() == false) {
				std::string roomType = currentRoom->type_();
				if (roomType == "empty") {
					std::cout << "An eerie silence fills the room..." << std::endl;
					player.changeStat("hp", 5, true);
					std::cout << "You recover 3 hp." << std::endl;
					floor.clearRoomAt(p_x, p_y);
				}
				if (roomType == "fight") {
					Character toFight = currentRoom->enemy_();
					fight(&player, &toFight);
					player.changeStat("hp",10, true);
					std::cout << "You recover 5 hp." << std::endl;
					floor.clearRoomAt(p_x, p_y);
				}
				if (roomType == "loot") {
					std::cout << "You found some loot!" << std::endl;
					Container loot = currentRoom->loot_();
					while (true) {
						size_t n;
						int upperBound = 0;
						for (n = 0; n < loot.numItems_(); n++) {
							std::cout << "[" << n + 1 << "]" << loot.viewItem(n)->name_() << std::endl;
							upperBound++;
						}
						std::cout << "[" << n + 1 << "]" << "Exit" << std::endl;
						std::cout << "Take item: ";
						int input = getInput(new int[2]{ 1,upperBound + 1 });
						if (input <= upperBound) {
							Item* toTake = loot.viewItem(input - 1);
							int idToTake = toTake->id_();
							loot.moveItemTo(idToTake, player.inventory_());
							std::cout << "You took the " + toTake->name_() << std::endl;
						}
						else {
							floor.clearRoomAt(p_x, p_y);
							break;
						}
					}
				}
			}
		}
		//equipment menu
		if (input == 2) {
			while (true) {
				int* eqBounds = player.printEquipmentMenu();
				std::cout << util::divider() << std::endl << "What do you want to do in your equipment?" << std::endl;
				std::cout << "[1]Inspect Item" << std::endl;
				std::cout << "[2]Unequip Item" << std::endl;
				std::cout << "[3]Exit" << std::endl;
				int input = getInput(new int[2]{ 1, 3 });
				std::cout << util::divider() << std::endl;
				if (input == 1) {
					std::cout << "Which item do you want to inspect?" << std::endl;
					int input = getInput(eqBounds);
					std::cout << util::divider() << std::endl;
					std::cout << player.inventory_()->peekItem(input - 1) << std::endl;
				}
				if (input == 2) {
					std::cout << "Which item do you want to unequip?" << std::endl;
					int input = getInput(eqBounds);
					std::cout << util::divider() << std::endl;
					int idToUnEquip = player.equipment_()->viewItem(input - 1)->id_();
					player.unequip(idToUnEquip);
					std::cout << util::divider() << std::endl;
				}
				if (input == 3) {
					break;
				}
			}
		}
		//inventory menu
		if (input == 3) {
			while (true) {
				int* invBounds = player.printInventoryMenu();
				std::cout << util::divider() << std::endl << "What do you want to do in your inventory?" << std::endl;
				std::cout << "[1]Inspect Item" << std::endl;
				std::cout << "[2]Equip Item" << std::endl;
				std::cout << "[3]Exit" << std::endl;
				int input = getInput(new int[2] {1, 3});
				std::cout << util::divider() << std::endl;
				if (input == 1) {
					std::cout << "Which item do you want to inspect?" << std::endl;
					int input = getInput(invBounds);
					std::cout << util::divider() << std::endl;
					std::cout << player.inventory_()->peekItem(input - 1) << std::endl;
				}
				if (input == 2) {
					std::cout << "Which item do you want to equip?" << std::endl;
					int input = getInput(invBounds);
					std::cout << util::divider() << std::endl;
					int idToEquip = player.inventory_()->viewItem(input-1)->id_();
					player.equip(idToEquip);
					std::cout << util::divider() << std::endl;
				}
				if (input == 3) {
					break;
				}
			}
		}
	}
}

void fight(Character* player, Character* enemy) {
	//fight introduction
	std::cout << "FIGHT: " << player->name_() << " VS. " << enemy->name_() << std::endl;
	std::cout << util::divider() << std::endl;

	int turnTimer[2]; //0=player, 1=enemy;
	//set turn timers
	turnTimer[0] = 100 - player->sp_();
	turnTimer[1] = 100 - enemy->sp_();

	//player stat change tracking
	std::vector<std::string> p_statsChanged;
	std::vector<int> p_amountsChanged;
	std::vector<int> p_timesChanged;
	//enemy stat change tracking
	std::vector<std::string> e_statsChanged;
	std::vector<int> e_amountsChanged;
	std::vector<int> e_timesChanged;
	
	bool fightRunning = true;
	//fight loop
	while (fightRunning) {
		//increment turn timers
		turnTimer[0]--;
		turnTimer[1]--;

		//player turn
		if (turnTimer[0] == 0) {
			//increment player stat changes
			unsigned n = 0;
			while(n < p_timesChanged.size()) {
				p_timesChanged.at(n)--;
				if (p_timesChanged.at(n) == 0) {
					std::string s = p_statsChanged.at(n);
					int i = p_amountsChanged.at(n);
					p_statsChanged.erase(p_statsChanged.begin() + n);
					p_amountsChanged.erase(p_amountsChanged.begin() + n);
					p_timesChanged.erase(p_timesChanged.begin() + n);
					player->changeStat(s, -i);
					std::cout << "(Your " << s << " returned to normal.) [" << -i << "]" << std::endl;
				}
				else {
					n++;
				}
			}
			player->regen();

			//display menu for ability selection
			std::cout << "Select an ability:" << std::endl;
			int* bounds = player->printAbilityMenu();

			//print stats: learn printf. Yes, it's a pain, but proper UI is good.
			std::cout << "PLAYER HP: " << player->hp_() << "/" << player->hp_max_();
			std::cout << "     ENEMY HP: " << enemy->hp_() << "/" << enemy->hp_max_();
			std::cout << "     TIME TILL NEXT MOVE: " << std::endl;
			std::cout << "PLAYER MP: " << player->mp_() << "/" << player->mp_max_();
			std::cout << "     ENEMY MP: " << enemy->mp_() << "/" << enemy->mp_max_();
			std::cout << "     PLAYER: " << turnTimer[0] << " ENEMY: " << turnTimer[1] <<std::endl;
			int selection = getInput(bounds);
			std::cout << util::divider() << std::endl;

			//get ability to use
			std::vector<std::vector<std::string>> changeTokens = player->getAbility(selection-1)->useAbility(player, enemy);

			//iterate through change tokens
			for (unsigned n = 0; n < changeTokens.size(); n++) {

				//get data from token
				std::vector<std::string> token = changeTokens.at(n);
				std::string statChanged = token.at(0);
				int amountChanged = std::stoi(token.at(1));
				int timeChanged = std::stoi(token.at(2));
				std::string changedFor = token.at(3);

				//assign stat changes
				if (changedFor == "self" && timeChanged > 0) {
					p_statsChanged.push_back(statChanged);
					p_amountsChanged.push_back(amountChanged);
					p_timesChanged.push_back(timeChanged);
				}
				if (changedFor == "other" && timeChanged > 0) {
					e_statsChanged.push_back(statChanged);
					e_amountsChanged.push_back(amountChanged);
					e_timesChanged.push_back(timeChanged);
				}
			}

			//reset turn timer
			turnTimer[0] = 100 - player->sp_();
		}

		//check hp values : exit condition
		if (player->hp_() == 0) {
			std::cout << "The light fades..." << std::endl;
			fightRunning = false;
			break;
		}
		if (enemy->hp_() == 0) {
			std::cout << "The enemy falls." << std::endl;
			fightRunning = false;
			break;
		}

		//enemy turn
		if (turnTimer[1] == 0) {
			//increment enemy stat changes
			unsigned n = 0;
			while (n < e_timesChanged.size()) {
				e_timesChanged.at(n)--;
				if (e_timesChanged.at(n) == 0) {
					std::string s = e_statsChanged.at(n);
					int i = e_amountsChanged.at(n);
					e_statsChanged.erase(e_statsChanged.begin() + n);
					e_amountsChanged.erase(e_amountsChanged.begin() + n);
					e_timesChanged.erase(e_timesChanged.begin() + n);
					enemy->changeStat(s, -i);
				}
				else {
					n++;
				}
			}
			enemy->regen();
			//pick random (?) move to use
			std::vector<std::vector<std::string>> changeTokens = enemy->getAbility(0)->useAbility(enemy, player);

			//iterate through change tokens
			for (unsigned n = 0; n < changeTokens.size(); n++) {

				//get data from token
				std::vector<std::string> token = changeTokens.at(n);
				std::string statChanged = token.at(0);
				int amountChanged = std::stoi(token.at(1));
				int timeChanged = std::stoi(token.at(2));
				std::string changedFor = token.at(3);

				//assign stat changes
				if (changedFor == "self" && timeChanged > 0) {
					e_statsChanged.push_back(statChanged);
					e_amountsChanged.push_back(amountChanged);
					e_timesChanged.push_back(timeChanged);
				}
				if (changedFor == "other" && timeChanged > 0) {
					p_statsChanged.push_back(statChanged);
					p_amountsChanged.push_back(amountChanged);
					p_timesChanged.push_back(timeChanged);
				}
			}
			//reset turn timer
			turnTimer[1] = 100 - enemy->sp_();
		}

		//check hp values : exit condition
		if (player->hp_() == 0) {
			std::cout << "The light fades..." << std::endl;
			fightRunning = false;
			break;
		}
		if (enemy->hp_() == 0) {
			std::cout << "The enemy falls." << std::endl;
			fightRunning = false;
			break;
		}
	}
}

int getInput(int* bounds) {
	int input;
	bool valid = false;
	while (not(valid)) {
		std::string temp;
		bool isInt = true;
		std::getline(std::cin, temp);
		for (size_t n = 0; n < temp.size(); n++) {
			if (temp[n] < 48 || temp[n] > 57) {
				isInt = false;
			}
		}
		if (isInt) {
			input = std::stoi(temp);
			if (input >= bounds[0] && input <= bounds[1]) {
				valid = true;
			}
		}
	}
	return input;
}