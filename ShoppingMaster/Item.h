#pragma once

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
	float mPrice;
	Temp mTemp;

public:
	Item()
	{


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
	float GetPrice() { return mPrice; }
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

	void SetPrice(float price)
	{
		mPrice = price;
	}

	void SetTemp(Temp temp)
	{
		mTemp = temp;
	}

	void PrintItem()
	{
		std::cout << mStore.mName << std::endl
			<< mStore.mAddress << std::endl
			<< mName << std::endl
			<< mCount << std::endl
			<< mPrice << std::endl
			<< mTemp << std::endl;
	}
};