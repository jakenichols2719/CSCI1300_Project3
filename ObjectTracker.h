#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Item.h"
#include "Container.h"
#include "Ability.h"
#include "util.h"

class ObjectTracker
{
	std::string filePath = "C:\\Users\\jaken\\source\\repos\\proj3\\";
	std::vector<Item*> items;
	std::vector<Container*> containers;
	std::vector<Ability*> abilities;
public:
	ObjectTracker(){};

	void readItems(std::string fileName);
	Item* getItem(int id);
	int getNumItems() { return items.size(); };

	void readContainers(std::string fileName);
	Container* getContainer(int id);
	int getNumContainers() { return containers.size(); };

	void readAbilities(std::string fileName);
	Ability* getAbility(int id);
	int getNumAbilities();

	//void readCharacters(std::string fileName);
};

