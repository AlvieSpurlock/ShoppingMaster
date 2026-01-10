#pragma once
#include "Console.h"
#include <windows.h>

void enableANSI()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

struct Store
{
	std::string mName;
	std::string mAddress;
};

enum Temp
{
	Room,
	Cold,
	Freezer
};

class Item
{
private:
	Store mStore;
	std::string mName;
	int mCount;
	double mPrice;
	Temp mTemp;

public:
	Item()
	{
		enableANSI();
		mStore.mName = "Input Store Name";
		mStore.mAddress = "Input Address";
		mName = "Input Item Name";
		mCount = 1;
		mPrice = 1.99;
		mTemp = Room;
	}

	std::string GetStoreName() { return mStore.mName; }
	std::string GetStoreAddress() { return mStore.mAddress; }
	std::string GetName() { return mName; }
	int GetCount() { return mCount; }
	double GetPrice() { return mPrice; }
	Temp GetTemp() { return mTemp; }

	void SetStore(std::string storeName, std::string storeAddress)
	{
		mStore.mName = storeName;
		mStore.mAddress = storeAddress;
	}

	void SetName(std::string name)
	{
		mName = name;
	}

	void SetCount(int count) 
	{ 
		mCount = count; 
	}

	void SetPrice(double price)
	{
		mPrice = price;
	}

	void SetTemp(Temp temp)
	{
		mTemp = temp;
	}

	void PrintItemDebug()
	{
		
		Console::SetForegroundColor(LightGrey);
		std::cout << "Store Name: "; 
		Console::SetForegroundColor(White);
		std::cout << mStore.mName;

		Console::SetForegroundColor(LightGrey);
		std::cout << " -- Store Address: ";
		Console::SetForegroundColor(White);
		std::cout << mStore.mAddress;

		Console::SetForegroundColor(LightGrey);
		std::cout << " -- Item Name: ";
		Console::SetForegroundColor(White);
		std::cout << mName;

		Console::SetForegroundColor(LightGrey);
		std::cout << " -- Item Count: ";
		Console::SetForegroundColor(White);
		std::cout << mCount;

		Console::SetForegroundColor(LightGrey);
		std::cout << " -- Item Price: ";
		Console::SetForegroundColor(White);
		std::cout << mPrice;

		switch (mTemp)
		{
		case Room:
			Console::SetForegroundColor(LightGrey);
			std::cout << " -- Item Temp: ";
			Console::SetForegroundColor(White);
			std::cout << "Room";
			break;
		case Cold:
			
			Console::SetForegroundColor(Cyan);
			std::cout << " -- Item Temp: ";
			Console::SetForegroundColor(White);
			std::cout << "Cold";
			break;
		case Freezer:
			std::cout << " -- ";
			Console::SetBackgroundColor(White);
			Console::SetForegroundColor(Blue);
			std::cout << "Item Temp:";
			Console::SetBackgroundColor(Black);
			Console::SetForegroundColor(LightGrey);
			std::cout << " Freezer";
			Console::SetForegroundColor(White);
			break;
		}
	}

	void PrintItem()
	{
		Console::SetForegroundColor(LightGrey);
		std::cout << " -- Item Name: ";
		Console::SetForegroundColor(White);
		std::cout << mName;

		Console::SetForegroundColor(LightGrey);
		std::cout << " -- Item Count: ";
		Console::SetForegroundColor(White);
		std::cout << mCount;

		Console::SetForegroundColor(LightGrey);
		std::cout << " -- Item Price: ";
		Console::SetForegroundColor(White);
		std::cout << mPrice;

		switch (mTemp)
		{
		case Room:
			Console::SetForegroundColor(LightGrey);
			std::cout << " -- Item Temp: ";
			Console::SetForegroundColor(White);
			std::cout << "Room";
			break;
		case Cold:

			Console::SetForegroundColor(Cyan);
			std::cout << " -- Item Temp: ";
			Console::SetForegroundColor(White);
			std::cout << "Cold";
			break;
		case Freezer:
			std::cout << " -- ";
			Console::SetBackgroundColor(White);
			Console::SetForegroundColor(Blue);
			std::cout << "Item Temp:";
			Console::SetBackgroundColor(Black);
			Console::SetForegroundColor(LightGrey);
			std::cout << " Freezer";
			Console::SetForegroundColor(White);
			break;
		}
	}
};