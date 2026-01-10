#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "Item.h"


std::vector<Item> mList;

void AddItem()
{
	std::vector<Item>::iterator it;
	std::cout << "Item Store Name: ";
	std::string newStoreName;
	std::cin >> newStoreName;

	std::cout << "Item Store Address: ";
	std::string newStoreAddress;
	std::cin >> newStoreAddress;

	std::cout << "Item Name: ";
	std::string newName;
	std::cin >> newName;

	std::cout << "Item Count: ";
	std::string sNewCount;
	std::cin >> sNewCount;
	int newCount = stoi(sNewCount);

	std::cout << "Item Price: ";
	std::string sNewPrice;
	std::cin >> sNewPrice;
	float newPrice = stof(sNewPrice);
	std::cout <<
		"Item Temp:\n1:Room\n2:Cold\n3:Freezer\n*Give Number\n";
	std::string sNewTemp;
	std::cin >> sNewTemp;
	int newTemp = stoi(sNewTemp);

	Item item;
	item.SetStore(newStoreName, newStoreAddress);
	item.SetName(newName);
	item.SetCount(newCount);
	item.SetPrice(newPrice);
	item.SetTemp(Temp(newTemp));

	mList.push_back(item);

}

void main()
{
	AddItem();
	for (auto& item : mList)
	{
		item.PrintItem();
	}
}