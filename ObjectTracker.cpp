#include "ObjectTracker.h"


void ObjectTracker::readItems(std::string fileName)
{
	std::ifstream inputFile(filePath + fileName);
	std::string tempData;
	while (std::getline(inputFile, tempData)) {
		std::vector<std::string> tokens = util::split(tempData, ',');
		std::string _type = tokens.at(0);
		if (_type == "item") {
			int _id = std::stoi(tokens.at(1));
			std::string _name = tokens.at(2);
			std::string _desc = tokens.at(3);
			int _value = std::stoi(tokens.at(4));
			Item* i = new Item(_type, _id, _name, _desc, _value);
			items.push_back(i);
		}
		else if (_type == "wep") {
			int _id = std::stoi(tokens.at(1));
			std::string _name = tokens.at(2);
			std::string _desc = tokens.at(3);
			int _value = std::stoi(tokens.at(4));
			Die _damageDie = tokens.at(5);
			int _bonus = std::stoi(tokens.at(6));
			Weapon* w = new Weapon(_type, _id, _name, _desc, _value, _damageDie, _bonus);
			items.push_back(w);
		}
		else if (_type == "arm") {
			int _id = std::stoi(tokens.at(1));
			std::string _name = tokens.at(2);
			std::string _desc = tokens.at(3);
			int _value = std::stoi(tokens.at(4));
			int _acBonus = std::stoi(tokens.at(5));
			Armor* a = new Armor(_type, _id, _name, _desc, _value, _acBonus);
			items.push_back(a);
		}
		else {

		}
	}
}

Item* ObjectTracker::getItem(int id)
{
	if (id < 0 || (size_t)id >= items.size()) {
		return items.at(0);
	}
	return items.at(id);
}

void ObjectTracker::readContainers(std::string fileName)
{
	std::ifstream inputFile(filePath + fileName);
	if (inputFile.good() == false) {
		std::cout << "ERROR: Container file could not be read." << std::endl;
		return;
	}
	std::string tempData;
	while (std::getline(inputFile, tempData)) {
		//split into container data and (contained) item data
		std::vector<std::string> dataTokens = util::split(tempData, '#');
		std::string containerData = dataTokens.at(0);
		std::string itemData = dataTokens.at(1);
		
		//basic container data
		std::vector<std::string> cDataTokens = util::split(containerData, ',');
		int _id = std::stoi(cDataTokens.at(0));
		std::string _name = cDataTokens.at(1);
		int _maxItems = std::stoi(cDataTokens.at(2));
		Container* c = new Container(_id, _name, this, _maxItems);

		std::vector<std::string> iDataTokens = util::split(itemData, '%');
		for (unsigned n = 0; n < iDataTokens.size(); n++) {
			std::vector<std::string> indivItemData = util::split(iDataTokens.at(n), ',');
			int itemid = std::stoi(indivItemData.at(0));
			int itemQuant = std::stoi(indivItemData.at(1));
			c->addItem(itemid, itemQuant);
		}

		containers.push_back(c);
	}
}

Container* ObjectTracker::getContainer(int id)
{
	if (id < 1 || (size_t)id >= containers.size()) {
		return containers.at(0);
	}
	return containers.at(id);
}

void ObjectTracker::readAbilities(std::string fileName)
{
	std::ifstream inputFile(filePath + fileName);
	if (inputFile.good() == false) {
		std::cout << "ERROR: Ability file could not be read." << std::endl;
		return;
	}

	std::string tempData;
	while (std::getline(inputFile, tempData)) {
		std::vector<std::string> tokens = util::split(tempData, '#');
		std::string abiData = tokens.at(0);
		std::string effData = tokens.at(1);

		std::vector<std::string> aDTokens = util::split(abiData, ',');
		int _id = std::stoi(aDTokens.at(0));
		std::string _name = aDTokens.at(1);
		Ability* a = new Ability(_id, _name);

		std::vector<std::string> eDTokens = util::split(effData, '%');
		for (unsigned n = 0; n < eDTokens.size(); n++) {
			std::vector<std::string> infoTokens = util::split(eDTokens.at(n), ',');
			std::string _target = infoTokens.at(0);
			std::string _targetstat = infoTokens.at(1);
			char _direction = infoTokens.at(2)[0];
			std::string _amount = infoTokens.at(3);
			int _time = std::stoi(infoTokens.at(4));
			a->addEffect(_target, _targetstat, _direction, _amount, _time);
		}

		abilities.push_back(a);
	}
}
