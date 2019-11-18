#include "Container.h"
#include "ObjectTracker.h"
#include <iostream>
#include <algorithm>

bool Container::addItem(int id, int quant)
{
	if (contained.size() == maxItems) {
		return false;
	}
	Item* toAdd = tracker->getItem(id);
	contained.push_back(toAdd);
	quantities.push_back(quant);
	return true;
}

std::string Container::peekItem(int index)
{
	if (index < 0 || (unsigned)index >= contained.size()) {
		return "container index out of bounds";
	}
	std::string result = contained.at(index)->stringRep();
	result += "\nQuantity: " + std::to_string(quantities.at(index)) + "\n";
	result += util::divider();
	return result;
}

Item* Container::viewItem(int index)
{
	if (index < 0 || index >= contained.size()) {
		return nullptr;
	}
	return contained.at(index);
}

Item* Container::removeItem(int itemid)
{
	Item* toRemove = tracker->getItem(itemid);
	int qValue = -1;
	for (unsigned n = 0; n < contained.size(); n++) {
		if (contained.at(n) == toRemove) {
			qValue = quantities.at(n);
		}
	}
	if (qValue == -1) {
		return nullptr;
	}
	
	contained.erase(std::remove(contained.begin(), contained.end(), toRemove), contained.end());
	quantities.erase(std::remove(quantities.begin(), quantities.end(), qValue), quantities.end());
	return toRemove;
}

bool Container::moveItemTo(int itemid, Container* newContainer)
{
	int itemIndex = -1;
	for (unsigned n = 0; n < contained.size(); n++) {
		if (contained.at(n)->id_() == itemid) {
			itemIndex = n;
		}
	}
	if (itemIndex == -1) {
		return false;
	}

	bool check = newContainer->addItem(itemid, quantities.at(itemIndex));
	if (check) {
		removeItem(itemid);
		return true;
	}
	else {
		return false;
	}
}
