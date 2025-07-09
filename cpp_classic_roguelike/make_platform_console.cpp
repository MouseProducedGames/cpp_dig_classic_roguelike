// this include
#include "platform.hpp"

// local imports
import console;

// std imports
import <memory>;

import sdl3_console;
#ifdef _CONSOLE
import win_console;
#endif // _CONSOLE

void make_platform_console(char width, char height, Platform platform)
{
	switch (platform)
	{
#ifdef _CONSOLE
	case Platform::Console:
		Console::_INSTANCE = std::make_shared<WinConsole>(width, height);
		break;
#endif // _CONSOLE
	case Platform::SDL3:
		Console::_INSTANCE = std::make_shared<SDL3Console>(width, height);

	}
}

void make_platform_console(char width, char height)
{
	make_platform_console(width, height, DEFAULT_PLATFORM);
}
