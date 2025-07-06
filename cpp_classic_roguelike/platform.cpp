#include "platform.hpp"

#ifdef WindowsBuild

import win_console;
import console;

void make_platform_console(char width, char height)
{
	Console::_INSTANCE = std::make_shared<WinConsole>(width, height);
	
}
#endif WindowsBuild // WindowsBuild