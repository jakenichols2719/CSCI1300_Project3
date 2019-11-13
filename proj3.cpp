// proj3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "ObjectTracker.h"
#include "Character.h"
#include "Ability.h"
#include "Map.h";

int main()
{
	srand(time(NULL));
	
	ObjectTracker o;
	o.readItems("items.txt");
	o.readContainers("containers.txt");
	o.readAbilities("abilities.txt");
	o.readCharacters("characters.txt");

	run();
}

void run() {
	//select character
	//exploration loop
	//fight
}

void fight(Character* player, Character* enemy) {
	/*
	Needs:
	Countdown for turns
	Menu
	Result of chosen action
	*/
}