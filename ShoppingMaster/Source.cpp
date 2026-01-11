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
	size_t vectorSize = list.size();
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

void Sort(std::vector<Item>& list)
{
	std::sort(list.begin(), list.end(),
		[](const Item& itemA, const Item& itemB)
		{
			std::string storeA = itemA.GetStoreName();
			std::string storeB = itemB.GetStoreName();

			for (auto& c : storeA) { c = std::toupper(c); }
			for (auto& c : storeB) { c = std::toupper(c); }

			if (storeA != storeB) { return storeA < storeB; }

			Temp tempA = itemA.GetTemp();
			Temp tempB = itemB.GetTemp();

			if (tempA != tempB) { return tempA < tempB; }

			std::string nameA = itemA.GetName();
			std::string nameB = itemB.GetName();

			for (auto& c : nameA) { c = std::toupper(c); }
			for (auto& c : nameB) { c = std::toupper(c); }

			return nameA < nameB;
		});
}


std::string GetStoreName()
{
	Console::SetForegroundColor(LightGrey);
	std::cout << "Store Name: ";
	Console::SetForegroundColor(White);
	std::string newStoreName;
	std::cin >> newStoreName;
	return newStoreName;
}

std::string GetStoreAddress()
{
	Console::SetForegroundColor(LightGrey);
	std::cout << "Store Address: ";
	Console::SetForegroundColor(White);
	std::string newStoreAddress;
	std::cin >> newStoreAddress;
	return newStoreAddress;
}

std::string GetItemName()
{
	Console::SetForegroundColor(LightGrey);
	std::cout << "Item Name: ";
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

int SelectStore()
{
	std::cout << "===Stores===\n";
	int index = 0;
	for (auto& store : mStores)
	{
		index++;
		std::cout << index << ": " << store[0] << " | " << store[1] << "\n";
	}
	std::cout << "Which Store? ";
	std::cin >> index;

	return index;
}

Item MakeItem()
{
	Item item;
	if (mStores.empty())
	{
		Console::SetForegroundColor(Red);
		std::cout << "No Stores are Listed. Please add a Store.\n";
	}
	else
	{
		int index = SelectStore();
		if (index < 0 || index > mStores.size())
		{
			Console::SetForegroundColor(Red);
			std::cout << "Invalid Input. Try Again.\n";
			return MakeItem();
		}
		else
		{
			std::string newStoreName = mStores[index - 1][0];
			std::string newStoreAddress = mStores[index - 1][1];
			std::string newName = GetItemName();
			int newCount = GetItemCount();
			double newPrice = GetItemPrice();
			int newTemp = GetItemTemp();

			item.SetStore(newStoreName, newStoreAddress);
			item.SetName(newName);
			item.SetCount(newCount);
			item.SetPrice(newPrice);
			item.SetTemp(Temp(newTemp - 1));
		}
	}
	return item;
}
	
void AddItem(Item item)
{
	mList.push_back(item);
	Sort(mList);
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

void MakeStore()
{
	std::vector<std::string> placeHolder;

	std::string newStoreName = GetStoreName();
	placeHolder.push_back(newStoreName);

	std::string newStoreAddress = GetStoreAddress();
	placeHolder.push_back(newStoreAddress);
	mStores.push_back(placeHolder);

	sBubbleSort(mStores);
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
	Sort(mList);
}

void SaveList()
{

	std::ofstream saveFile("SavedList.txt");
	int index = 0;
	for (auto& store : mStores)
	{
		if (index > 0) { saveFile << "\n\n"; }
		index++;
		std::string StoreName = "=====" + store[0] + " - " + store[1] + "=====\n";
		saveFile << StoreName;

		for (auto& item : mList)
		{
			if (item.GetStoreName() == store[0])
			{
				saveFile << "Temp: ";
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

				saveFile << "Name: " << item.GetName() << " -- "
					<< "Count: " << item.GetCount() << "\n"
					<< "Price: " << item.GetPrice() << "\n";
				for (int index = 0; index < StoreName.length() + 5; index++)
				{
					saveFile << "=";
				}
				saveFile << "\n";
			}
		}
	}
	saveFile.close();
}

void UserInput()
{
	while (true)
	{
		Console::SetForegroundColor(White);
		std::cout << "----Options----\n";
		Console::SetForegroundColor(White);
		std::cout << "1: ";
		Console::SetForegroundColor(Green);
		std::cout << "Print Shopping List\n";
		Console::SetForegroundColor(White);
		std::cout << "2: ";
		Console::SetForegroundColor(Green);
		std::cout << "Add Item\n";
		Console::SetForegroundColor(White);
		std::cout << "3: ";
		Console::SetForegroundColor(Green);
		std::cout << "Add Store\n";
		Console::SetForegroundColor(White);
		std::cout << "4: ";
		Console::SetForegroundColor(Green);
		std::cout << "Print Stores\n";
		Console::SetForegroundColor(White);
		std::cout << "5: ";
		Console::SetForegroundColor(Green);
		std::cout << "Replace Item\n";
		Console::SetForegroundColor(White);
		std::cout << "6: ";
		Console::SetForegroundColor(Green);
		std::cout << "Save List\n";
		Console::SetForegroundColor(White);
		int userIn = GetIndex("Answer? ");

		// Validate input
		if (userIn < 1 || userIn > 6)
		{
			Console::SetForegroundColor(Red);
			std::cout << "Invalid Input. Try Again.\n";
			continue; // ask again
		}

		switch (userIn)
		{
		case 1:
			if (mList.empty()) 
			{ Console::SetForegroundColor(Red); std::cout << "List has no Items. Please add at least one Item.\n"; }
			else
			{
				for (size_t index = 0; index < mStores.size(); index++)
				{
					std::cout << mStores[index][0] << "===" << mStores[index][1] << "\n";
					for (auto& mItem : mList)
					{
						if (mItem.GetStoreName() == mStores[index][0])
							mItem.PrintItem();
					}
				}
			}
			break;
			

		case 2:
		{
			Item nItem = MakeItem();
			AddItem(nItem);
			Sort(mList);
			for (auto& item : mList)
			{ if (item.GetName() != "Input Item Name") { item.PrintItem(); } }
			break;
		}

		case 3:
			MakeStore();
			break;

		case 4:
		{
			std::cout << "===Stores===\n";
			int index = 0;
			for (auto& store : mStores)
			{
				index++;
				std::cout << index << ": " << store[0] << " | " << store[1] << "\n";
			}
			break;
		}


		case 5:
		{
			Item rItem = MakeItem();
			ReplaceItem(rItem);
			Sort(mList);
			break;
		}

		case 6:
			SaveList();
			return;
		}

	}
}


int main()
{
	enableANSI();
	UserInput();
	return 0;
}