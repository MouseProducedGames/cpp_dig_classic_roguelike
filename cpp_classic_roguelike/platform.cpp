#include "platform.hpp"

#ifdef WindowsBuild

#include "platforms/win32/win_console.hpp"

std::shared_ptr<Console> make_platform_console(char width, char height)
{
	return std::make_shared<WinConsole>(width, height);
	
}
#endif WindowsBuild // WindowsBuild