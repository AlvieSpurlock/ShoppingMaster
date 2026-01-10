#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string_view>
#include <algorithm> 
#include <cctype>
#include <windows.h>

#include "Item.h"

std::vector<Item> mList;
std::vector<std::vector<std::string>> mStores;

void sBubbleSort(std::vector<std::vector<std::string>>& list)
{
	int vectorSize = list.size();
	bool swapped;

	do {
		swapped = false;

		for (int index = 1; index < vectorSize; index++)
		{
			if (_stricmp(list[index - 1][0].c_str(), list[index][0].c_str()) > 0)
			{
				std::swap(list[index], list[index - 1]);
				swapped = true;
			}
		}

		vectorSize--;

	} while (swapped);
}


void BubbleSort(std::vector<Item>& list)
{
	int vectorSize = list.size();
	bool swapped;

	do {
		swapped = false;

		for (int index = 1; index < vectorSize; index++)
		{
			std::string Name1 = list[index - 1].GetStoreName();
			std::string Name2 = list[index].GetStoreName();

			for (auto& character : Name1) character = std::toupper(character);
			for (auto& character : Name2) character = std::toupper(character);

			if (Name1 > Name2)
			{
				std::swap(list[index], list[index - 1]);
				swapped = true;
			}
		}

		vectorSize--;

	} while (swapped);
}

std::string GetStoreName()
{
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Store Name: ";
	Console::SetForegroundColor(White);
	std::string newStoreName;
	std::cin >> newStoreName;
	return newStoreName;
}

std::string GetStoreAddress()
{
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Store Address: ";
	Console::SetForegroundColor(White);
	std::string newStoreAddress;
	std::cin >> newStoreAddress;
	return newStoreAddress;
}

std::string GetItemName()
{
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Item Name: ";
	Console::SetForegroundColor(White);
	std::string newItemName;
	std::cin >> newItemName;
	return newItemName;
}

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

Item MakeItem()
{
	std::vector<Item>::iterator it;

	std::string newStoreName = GetStoreName();
	std::string newStoreAddress = GetStoreAddress();
	std::string newName = GetItemName();
	int newCount = GetItemCount();
	double newPrice = GetItemPrice();
	int newTemp = GetItemTemp();

	std::vector<std::string> placeHolder;
	placeHolder.push_back(newStoreName);

	placeHolder.push_back(newStoreAddress);
	mStores.push_back(placeHolder);
	sBubbleSort(mStores);

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
	BubbleSort(mList);
}
int GetIndex(std::string inputText)
{
	int index = 0;
	Console::SetForegroundColor(LightGrey);
	std::cout << inputText;
	Console::SetForegroundColor(White);
	std::string sIndex;
	std::cin >> sIndex;
	if (std::all_of(sIndex.begin(), sIndex.end(), ::isdigit))
	{
		if (stoi(sIndex) > 0)
		{
			index = stoi(sIndex);
		}
		else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemCount(); }
	}
	else { Console::SetForegroundColor(Red); std::cout << "Invalid Input. Try Again." << std::endl; GetItemCount(); }

	return index;
}
void ReplaceItem(Item newItem)
{
	int index = 0;
	for (auto& item : mList)
	{
		index++;
		std::cout << index;
		item.PrintItem();
	}
	mList[GetIndex("Item Index: ") - 1] = newItem;
	BubbleSort(mList);
}

void SaveList()
{
	std::ofstream saveFile("SavedList.txt");
	for (auto& item : mList)
	{
		saveFile << item.GetStoreName() << " - " << item.GetStoreAddress() << " | "
			<< "Name: " << item.GetName() << " --- "
			<< "Count: " << item.GetCount() << " --- "
			<< "Price: " << item.GetPrice() << " --- Temp: ";
		switch (item.GetTemp())
		{
		case Room:
			saveFile << "Room\n";
			break;
		case Cold:

			saveFile << "Cold\n";
			break;
		case Freezer:
			saveFile << "Freezer\n";
			break;
		}
	}
	saveFile.close();
}

void UserInput()
{
	while (true)
	{
		Console::SetForegroundColor(Green);
		std::cout << "----Options----\n1:Print List\n2:Add Item\n3:Replace Item\n4:Save List\n";
		Console::SetForegroundColor(White);
		int userIn = GetIndex("Answer? ");

		// Validate input
		if (userIn < 1 || userIn > 4)
		{
			Console::SetForegroundColor(Red);
			std::cout << "Invalid Input. Try Again.\n";
			continue; // ask again
		}

		switch (userIn)
		{
		case 1:
			for (size_t index = 0; index < mStores.size(); index++)
			{
				std::cout << mStores[index][0] << "===" << mStores[index][1] << "\n";
				for (auto& item : mList)
				{
					if (item.GetStoreName() == mStores[index][0])
						item.PrintItem();
				}
			}
			break;

		case 2:
		{
			Item item = MakeItem();
			AddItem(item);
			BubbleSort(mList);
			for (auto& item : mList) item.PrintItem();
			break;
		}

		case 3:
		{
			Item item = MakeItem();
			ReplaceItem(item);
			BubbleSort(mList);
			break;
		}

		case 4:
			SaveList();
			return;
		}
	}
}


void main()
{
	enableANSI();
	UserInput();
}