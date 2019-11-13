// proj3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "ObjectTracker.h"
#include "Character.h"
#include "Ability.h"
#include "Map.h"

void run();
void fight(Character*, Character*);

int main()
{
	srand(time(NULL));
	
	ObjectTracker o;
	o.readItems("items.txt");
	o.readContainers("containers.txt");
	o.readAbilities("abilities.txt");
	o.readCharacters("characters.txt");

	Character player = o.getCharacter(0)->instance();
	Character skelly = o.getCharacter(2)->instance();
	fight(&player, &skelly);
}

void run() {
	//select character
	//exploration loop
	//fight
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

		//increment stat changes

		//player turn
		if (turnTimer[0] == 0) {
			std::vector<std::vector<std::string>> changeTokens = player->getAbility(2)->useAbility(player, enemy);

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

		//check hp values

		//enemy turn
		if (turnTimer[1] == 0) {
		}

		//check hp values
	}
}