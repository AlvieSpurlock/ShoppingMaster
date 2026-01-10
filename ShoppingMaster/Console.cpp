#include "Console.h"
#include <iostream>

using namespace std;


const string Console::ESC = "\x1B";
int Console::windowWidth = 100, Console::windowHeight = 50;

void Console::SetForegroundColor(ConsoleColor foreColor)
{
	cout << ESC << "[" << foreColor + 30 << "m";
}

void Console::SetForegroundColor(int r, int g, int b)
{
	cout << ESC << "[38;2;" << r << ";" << g << ";" << b << "m";
}

void Console::SetBackgroundColor(ConsoleColor backColor)
{
	cout << ESC << "[" << backColor + 40 << "m";
}

void Console::SetBackgroundColor(int r, int g, int b)
{
	cout << ESC << "[48;2;" << r << ";" << g << ";" << b << "m";
}