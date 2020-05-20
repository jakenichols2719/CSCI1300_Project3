#include "Container.h"
#include "ObjectTracker.h"
#include <iostream>
#include <algorithm>

bool Container::addItem(int id, int quant)
{
	if (contained.size() == maxItems) {
		return false;
	}
	bool alreadyContained = false;
	unsigned n;
	for (n = 0; n < contained.size(); n++) {
		if (contained.at(n)->id_() == id) {
			alreadyContained = true;
			break;
		}
	}
	if (alreadyContained) {
		quantities.at(n)++;
	}
	else {
		Item* toAdd = tracker->getItem(id);
		contained.push_back(toAdd);
		quantities.push_back(quant);
	}
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

Item* Container::removeItem(int itemid, int quant)
{
	Item* toRemove = tracker->getItem(itemid);
	bool found = false;
	unsigned n;
	for (n = 0; n < contained.size(); n++) {
		if (contained.at(n)->id_() == itemid) {
			found = true;
			break;
		}
	}
	if (found != true) {
		return nullptr;
	}

	if (quant == -1) {
		contained.erase(contained.begin() + n);
		quantities.erase(quantities.begin() + n);
	}
	else {
		quantities.at(n)--;
		if (quantities.at(n) <= 0) {
			contained.erase(contained.begin() + n);
			quantities.erase(quantities.begin() + n);
		}
	}
	return toRemove;
}

bool Container::moveItemTo(int itemid, Container* newContainer, int quant)
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

	if (quant == -1) {
		bool check = newContainer->addItem(itemid, quantities.at(itemIndex));
		if (check) {
			removeItem(itemid, -1);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		bool check = newContainer->addItem(itemid, quant);
		if (check) {
			removeItem(itemid, quant);
			return true;
		}
		else {
			return false;
		}
	}
}
