#pragma once
#include <string>

struct Store
{
	char* mName = nullptr;
	char* mAddress = nullptr;

	Store(char* name, char* address)
	{
		size_t nLen = strlen(name) + 1;
		mName = new char[nLen];
		strcpy_s(mName, nLen, name);

		size_t aLen = strlen(address) + 1;
		mAddress = new char[aLen];
		strcpy_s(mAddress, aLen, address);
	}

	char* GetName() { return mName; }
	char* GetAddress() { return mAddress; }
};