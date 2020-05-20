// proj3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "ObjectTracker.h"
#include "Character.h"
#include "Ability.h"
#include "Map.h"

void run(ObjectTracker* tracker);
bool fight(Character*, Character*);
int getInput(int* bounds);
int ai_basic_move(Character* c, int* stage);
int ai_cc_move(Character* boss, int* stage);
int ai_th_move(Character* boss, int* stage);
void endgame(int score);

int main()
{
	srand(time(NULL));
	
	ObjectTracker o;
	o.readItems("items.txt");
	o.readContainers("containers.txt");
	o.readAbilities("abilities.txt");
	o.readCharacters("characters.txt");
	o.readMaps("maps.txt");

	run(&o);
}

void run(ObjectTracker* tracker) {
	//select character
	std::cout << "Choose a character: " << std::endl;
	std::cout << "[1]Mad Monk" << std::endl;
	std::cout << "[2]Cultist" << std::endl;
	std::cout << "[3]Runeblade" << std::endl;
	int playerChoice = getInput(new int[2]{ 1,3 });
	Character player = tracker->getCharacter(playerChoice-1)->instance();
	//set up map
	int mapChoice = rand() % 2;
	Map floor = tracker->getMap(mapChoice)->instance();
	//character-based loot options
	switch (playerChoice - 1) {
	case 0: floor.addValidLoot(13); break;//Monk
	case 1: floor.addValidLoot(12); break;//Cultist
	case 2: floor.addValidLoot(14); break;//Runeblade
	}
	//map boss
	switch (floor.id_()) {
	case 0: floor.addBoss(5); break;
	case 1: floor.addBoss(8); break;
	}
	floor.fillRooms(tracker);

	//loop variables
	bool running = true;
	int p_x = 0;
	int p_y = 0;
	floor.clearRoomAt(0, 0);
	while (running) {
		floor.printMap(p_x, p_y);
		std::cout << "Choose an action:" << std::endl;
		int bounds[]{ 1,4 };
		std::cout << "[1]Move" << std::endl;
		std::cout << "[2]View Equipment" << std::endl;
		std::cout << "[3]View Inventory" << std::endl;
		std::cout << "[4]View Abilities" << std::endl;
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
			//already cleared
			if (currentRoom->clear_()) {
				std::cout << "You've been here before... The room is empty." << std::endl;
				std::cout << util::divider() << std::endl;
			}
			//boss fight
			if (floor.roomsCleared_() == 15) {
				//boss fight
				std::cout << "A boss approaches..." << std::endl;
				Character toFight = tracker->getCharacter(floor.boss_())->instance();
				bool alive = fight(&player, &toFight);
				if (alive) {
					std::cout << "You have cleared the run! Congratulations!" << std::endl;
					std::cout << util::divider() << std::endl;
				}
				break;
			}
			//not cleared
			if (currentRoom->clear_() == false) {
				std::string roomType = currentRoom->type_();
				if (roomType == "empty") {
					std::cout << "An eerie silence fills the room..." << std::endl;
					player.changeStat("hp", 5, true);
					std::cout << "You recover 5 hp." << std::endl;
					floor.clearRoomAt(p_x, p_y);
				}
				if (roomType == "fight") {
					Character toFight = currentRoom->enemy_();
					bool playerWon = fight(&player, &toFight);
					if (playerWon) {
						player.changeStat("hp", 5, true);
						std::cout << "You recover 5 hp." << std::endl;
						floor.clearRoomAt(p_x, p_y);
					}
					else {
						break;
					}
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
						if (input >= 0 && input <= upperBound) {
							Item* toTake = loot.viewItem(input - 1);
							int idToTake = toTake->id_();
							loot.moveItemTo(idToTake, player.inventory_(), -1);
							std::cout << "You took the " + toTake->name_() << std::endl;
						}
						else {
							floor.clearRoomAt(p_x, p_y);
							break;
						}
					}
					player.changeStat("hp", 5, true);
					std::cout << "You recover 5 hp." << std::endl;
				}
				std::cout << util::divider() << std::endl;
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
					std::cout << player.equipment_()->peekItem(input - 1) << std::endl;
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
		if (input == 4) {
			for (int n = 0; n < player.numAbilities_(); n++) {
				std::cout << player.getAbility(n)->stringRep() << std::endl;
				std::cout << "---" << std::endl;
			}
			std::cout << util::divider() << std::endl;
		}
	}
	//game end process
	int totalItemValue = 0;
	for (int n = 0; n < player.inventory_()->numItems_(); n++) {
		int tempVal = player.inventory_()->viewItem(n)->value_();
		totalItemValue += tempVal;
	}
	for (int n = 0; n < player.equipment_()->numItems_(); n++) {
		int tempVal = player.equipment_()->viewItem(n)->value_();
		totalItemValue += tempVal;
	}
	endgame(totalItemValue);
}

bool fight(Character* player, Character* enemy) {
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
	//enemy stage tracking
	int fightStage = -1;
	
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
					player->changeStat(s, -i, true);
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
			return false;
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
					enemy->changeStat(s, -i, true);
				}
				else {
					n++;
				}
			}
			enemy->regen();
			//pick random (?) move to use
			int moveChoice = 0;
			if (enemy->id_() == 5) {
				moveChoice = ai_cc_move(enemy, &fightStage);
			}
			else if (enemy->id_() == 8) {
				moveChoice = ai_th_move(enemy, &fightStage);
			}
			else {
				moveChoice = ai_basic_move(enemy, &fightStage);
			}
			std::vector<std::vector<std::string>> changeTokens = enemy->getAbility(moveChoice)->useAbility(enemy, player);

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
			return false;
			break;
		}
		if (enemy->hp_() == 0) {
			std::cout << "The enemy falls." << std::endl;
			fightRunning = false;
			break;
		}
	}
	//return stats to normal
	unsigned n = 0;
	while (n < p_statsChanged.size()) {
		std::string s = p_statsChanged.at(n);
		int i = p_amountsChanged.at(n);
		p_statsChanged.erase(p_statsChanged.begin() + n);
		p_amountsChanged.erase(p_amountsChanged.begin() + n);
		p_timesChanged.erase(p_timesChanged.begin() + n);
		player->changeStat(s, -i, true);
		std::cout << "(Your " << s << " returned to normal.) [" << -i << "]" << std::endl;
	}
	return true;
}

int getInput(int* bounds) {
	int input;
	bool valid = false;
	while (not(valid)) {
		std::string temp;
		bool isInt = true;
		std::getline(std::cin, temp);
		if (temp == "") {
			valid = false;
		}
		else {
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
	}
	return input;
}

int ai_basic_move(Character* c, int* stage)
{
	int n = 0;
	while (true) {
		Ability* a = c->getAbility(n);
		if (a == nullptr) {
			break;
		}
		n++;
	}
	int moveChoice = rand() % n;
	return moveChoice;
}

int ai_cc_move(Character* boss, int* stage)
{
	if (*stage == -1) {
		std::cout << "As you approach the massive Jet Zombie, it lets out a massive roar." << std::endl;
		*stage += 1;
		return 0;
	}
	if (*stage == 0) {
		int choice = (rand() % 2) + 1;
		if (boss->hp_() <= 40) {
			*stage += 1;
		}
		return choice;
	}
	if (*stage == 1) {
		std::cout << "Whatever this thing is, you've made it mad. It bellows as it prepares to rush you." << std::endl;
		*stage += 1;
		return 0;
	}
	if (*stage == 2) {
		int temp = rand() % 2;
		if (boss->hp_() <= 15) {
			*stage += 1;
		}
		if (temp == 0) return 1;
		if (temp == 1) return 3;
	}
	if (*stage == 3) {
		std::cout << "You look on in horror az the zombie begins to fall apart. A glow comes from the engine embedded in its chest...";
		*stage += 1;
		return 4;
	}
	if (*stage == 4) {
		return 5;
	}
	return 0;
}

int ai_th_move(Character* boss, int* stage)
{
	if (*stage == -1) {
		std::cout << "The building around you begins to rumble. It occurs to you that, possibly, you made a mistake in coming here." << std::endl;
		std::cout << "As you have this thought, swamp water floods the room." << std::endl;
		*stage += 1;
		return 5;
	}
	if (*stage == 0) {
		int temp = rand() % 2;
		if (boss->hp_() <= 25) {
			*stage += 1;
		}
		if (temp == 0) return 0;
		if (temp == 1) return 2;
	}
	if (*stage == 1) {
		std::cout << "The Herbarium quakes as you do away with the vicious plants. Suddenly, the overgrowth seems to resurge." << std::endl;
		*stage += 1;
		return 6;
	}
	if (*stage == 2) {
		int temp = rand() % 3;
		if (boss->hp_() <= 15) {
			*stage += 1;
		}
		if (temp == 0) return 1;
		if (temp == 1) return 3;
		if (temp == 2) return 4;
	}
	return 0;
}

void endgame(int score) {
	std::cout << "Score: " << score;
	std::string initials[5];
	int highScores[5];
	std::ifstream inputFile("highscores.txt");
	std::string tempData;
	//read scores
	for (int n = 0; n < 5; n++) {
		std::getline(inputFile, tempData);
		std::vector<std::string> tokens = util::split(tempData, ',');
		initials[n] = tokens.at(0);
		highScores[n] = std::stoi(tokens.at(1));
	}
	//check high score
	int scoreIndex = -1;
	for (int n = 0; n < 5; n++) {
		if (score > highScores[n]) {
			scoreIndex = n;
			break;
		}
	}
	//enter new initials
	if (scoreIndex != -1) {
		std::string newInitials;
		std::cout << "Enter your initials (3): " << std::endl;
		while (true) {
			std::cin >> newInitials;
			if (newInitials.size() == 3) {
				break;
			}
			else {
				std::cout << "Please enter valid initials." << std::endl;
			}
		}
		initials[scoreIndex] = newInitials;
		highScores[scoreIndex] = score;
	}
	//print (new) high scores
	std::cout << "HIGH SCORES" << std::endl;
	for (int n = 0; n < 5; n++) {
		std::cout << n+1 << " (" << initials[n] << "): " << highScores[n] << std::endl;;
	}
	inputFile.close();

	std::ofstream outputFile("highscores.txt", std::ofstream::out | std::ofstream::trunc);
	for (int n = 0; n < 5; n++) {
		outputFile << initials[n] + "," + std::to_string(highScores[n]) << std::endl;
	}
	outputFile.close();
}
