#pragma once
// Copyright © Mad Tea Party Development 2025
// Proprietary software - see license.

#include <string>
#include <vector>

enum Temp { Room = 0, Cold = 1, Freezer = 2 };

struct Store
{
    std::string name;
    std::string address;
};

struct Item
{
    Store       store;
    std::string name;
    int         count   = 1;
    double      price   = 0.0;   // total (price * count)
    Temp        temp    = Room;

    double UnitPrice() const { return count > 0 ? price / count : price; }
};
