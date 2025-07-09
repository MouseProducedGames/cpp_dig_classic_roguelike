// this include
#include "platform.hpp"

// local imports
import console;

// std imports
import <memory>;

#ifdef WindowsBuild

import win_console;

void make_platform_console(char width, char height, Platform platform)
{
	Console::_INSTANCE = std::make_shared<WinConsole>(width, height);
	
}

#elifdef SDL3Build

import sdl3_console;

void make_platform_console(char width, char height, Platform platform)
{
	Console::_INSTANCE = std::make_shared<SDL3Console>(width, height);

}

#endif WindowsBuild // WindowsBuild

void make_platform_console(char width, char height)
{
	make_platform_console(width, height, DEFAULT_PLATFORM);
}
