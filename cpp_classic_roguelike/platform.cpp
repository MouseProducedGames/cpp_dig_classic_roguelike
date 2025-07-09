#include "platform.hpp"

import console;

#ifdef WindowsBuild

import win_console;

void make_platform_console(char width, char height)
{
	Console::_INSTANCE = std::make_shared<WinConsole>(width, height);
	
}

#elifdef SDL3Build

import sdl3_console;

void make_platform_console(char width, char height)
{
	Console::_INSTANCE = std::make_shared<SDL3Console>(width, height);

}

#endif WindowsBuild // WindowsBuild