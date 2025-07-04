#include "platform.hpp"

#ifdef WindowsBuild

import win_console;
import console;

std::shared_ptr<Console> make_platform_console(char width, char height)
{
	return std::make_shared<WinConsole>(width, height);
	
}
#endif WindowsBuild // WindowsBuild