// proj3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "ObjectTracker.h"
#include "Character.h"
#include "Ability.h"

int main()
{
	srand(time(NULL));
	
	ObjectTracker o;
	o.readItems("items.txt");
	o.readContainers("containers.txt");
	o.readAbilities("abilities.txt");

	Character* c = new Character(0, "Player", 10, 10, 50, 10, o.getContainer(1), o.getContainer(2));
	Character* e = new Character(0, "Enemy", 10, 10, 50, 10, o.getContainer(3), o.getContainer(4));
}