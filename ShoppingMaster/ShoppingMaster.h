#pragma once
#include <iostream>

enum Temp
{
	Room,
	Cold,
	Freezer
};

class Item
{
private:
	float* mPrice = nullptr;
	int* mCount = nullptr;
	Temp* mTemp = nullptr;
	char* mStore = nullptr;

public:

};