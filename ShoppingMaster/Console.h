#pragma once
#include <string>
#include <iostream>


enum ConsoleColor
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    LightGrey = 60,
    Default = 9
};

class Console
{
public:
    static void SetForegroundColor(ConsoleColor foreColor);
    static void SetForegroundColor(int r, int g, int b);
    static void SetBackgroundColor(ConsoleColor backColor);
    static void SetBackgroundColor(int r, int g, int b);

private:
    static const std::string ESC;

    static int windowWidth, windowHeight;
};

