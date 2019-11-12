#pragma once
#include <string>
#include <vector>
#include "util.h"
#include "Item.h"

class ObjectTracker;

class Container
{
private:
	int id;
	std::string name;
	ObjectTracker* tracker;
	std::vector<Item*> contained;
	std::vector<int> quantities;
	int maxItems;
public:
	Container(int _id, std::string _name, ObjectTracker* _tracker, int _maxItems) : id(_id), name(_name), tracker(_tracker), maxItems(_maxItems) {};
	int id_() { return id; };
	std::string name_() { return name; };

	bool addItem(int id, int quant);
	std::string peekItem(int index);
	Item* viewItem(int index);
	Item* removeItem(int id);

	int numItems_() { return contained.size(); };

	bool moveItemTo(int itemid, Container* newContainer);

	Container instance() { return *this; };
};

