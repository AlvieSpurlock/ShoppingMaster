#pragma once

enum Temp
{
	Room,
	Cold,
	Freezer
};

class Item
{
private:
	char* mStore = nullptr;
	char* mName = nullptr;
	int* mCount = nullptr;
	float* mPrice = nullptr;
	Temp* mTemp = nullptr;

public:
	Item()
	{


		mStore = new char[15];
		strcpy_s(mStore, 15, "Input Store Name");
		mName = new char[15];
		strcpy_s(mName, 15, "Input Item Name");
		mCount = new int(1);
		mPrice = new float(1.99);
		mTemp = new Temp(Room);
	}

	Item(char* store, char* name, int count, float price, Temp temp)
	{
		size_t sLen = strlen(store) + 1;
		mStore = new char[sLen];
		strcpy_s(mStore, sLen, store);

		size_t nLen = strlen(name) + 1;
		mName = new char[nLen];
		strcpy_s(mName, nLen, name);

		mCount = new int(count);
		mPrice = new float(price);
		mTemp = new Temp(temp);
	}

	~Item()
	{
		delete[] mStore;
		delete[] mName;
		delete mCount;
		delete mPrice;
		delete mTemp;
	}

	char* GetStore() { return mStore; }
	char* GetName() { return mName; }
	int GetCount() { return *mCount; }
	float GetPrice() { return *mPrice; }
	Temp GetTemp() { return *mTemp; }

	void SetStore(char* store)
	{
		size_t sLen = strlen(store) + 1;
		mStore = new char[sLen];
		strcpy_s(mStore, sLen, store);
	}

	void SetName(char* name)
	{
		size_t nLen = strlen(name) + 1;
		mName = new char[nLen];
		strcpy_s(mName, nLen, name);
	}

	void SetCount(int count) 
	{ 
		delete mCount;  
		mCount = new int(count); 
	}

	void SetPrice(float price)
	{
		delete mPrice;
		mPrice = new float(price);
	}

	void SetTemp(Temp temp)
	{
		delete mTemp;
		mTemp = new Temp(temp);
	}
};