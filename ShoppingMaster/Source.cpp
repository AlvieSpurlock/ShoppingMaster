#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 
#include <cctype>
#include <windows.h>

#include "Item.h"

int GetItemCount()
{
	int newCount = 0;
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Count: ";
	Console::SetForegroundColor(White);
	std::string sNewCount;
	std::cin >> sNewCount;
	if (std::all_of(sNewCount.begin(), sNewCount.end(), ::isdigit))
	{
		if (stoi(sNewCount) > 0)
		{
			newCount = stoi(sNewCount);
		}
		else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemCount(); }
	}
	else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemCount(); }

	return newCount;
}

int GetItemTemp()
{
	int newTemp = 0;
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Temp:\n1:Room\n2:Cold\n3:Freezer\n*Give Number\n";
	Console::SetForegroundColor(White);
	std::string sNewTemp;
	std::cin >> sNewTemp;
	if (std::all_of(sNewTemp.begin(), sNewTemp.end(), ::isdigit))
	{
		if (stoi(sNewTemp) >= 1)
		{
			if (stoi(sNewTemp) <= 3) { newTemp = stoi(sNewTemp); }
			else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemTemp(); }
		}
		else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemTemp(); }
	}
	else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemTemp(); }
	
	return newTemp;
}

bool isValidDouble(const std::string& stringPlaceHolder)
{
	bool decimalFound = false;

	for (char character : stringPlaceHolder)
	{
		if (character == '.')
		{
			if (decimalFound) return false;
			decimalFound = true;
		}
		else if (!std::isdigit(character))
		{
			return false;
		}
	}

	return !stringPlaceHolder.empty();
}


double GetItemPrice()
{
	double newPrice = 0;
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Price: ";
	Console::SetForegroundColor(White);
	std::string sNewPrice;
	std::cin >> sNewPrice;
	if (isValidDouble(sNewPrice))
	{
		if (stod(sNewPrice) > 0)
		{
			newPrice = stod(sNewPrice);
		}
		else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemPrice(); }
	} 
	else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemPrice(); }

	return newPrice;
}
std::vector<Item> mList;
Item MakeItem()
{
	std::vector<Item>::iterator it;
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Store Name: ";
	Console::SetForegroundColor(White);
	std::string newStoreName;
	std::cin >> newStoreName;

	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Store Address: ";
	Console::SetForegroundColor(White);
	std::string newStoreAddress;
	std::cin >> newStoreAddress;

	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Name: ";
	Console::SetForegroundColor(White);
	std::string newName;
	std::cin >> newName;

	int newCount = GetItemCount();
	double newPrice = GetItemPrice();
	int newTemp = GetItemTemp();

	Item item;
	item.SetStore(newStoreName, newStoreAddress);
	item.SetName(newName);
	item.SetCount(newCount);
	item.SetPrice(newPrice);
	item.SetTemp(Temp(newTemp - 1));

	return item;
}
void AddItem(Item item)
{
	mList.push_back(item);

}
int GetIndex()
{
	int index = 0;
	for (auto& item : mList)
	{
		item.PrintItem();
	}
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Name: ";
	std::cin >> index;
	return index;
}
void ReplaceItem()
{
	for (auto& item : mList)
	{
		item.PrintItem();
	}
}

void main()
{
	enableANSI();
	AddItem(MakeItem());
	for (auto& item : mList)
	{
		item.PrintItem();
	}
	
}