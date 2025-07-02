#include "platform.hpp"

#ifdef WindowsBuild

#include "platforms/win32/win_console.hpp"

std::shared_ptr<Console> make_platform_console()
{
	return std::make_shared<WinConsole>();
	
}
#endif WindowsBuild // WindowsBuild