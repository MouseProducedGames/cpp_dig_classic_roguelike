#pragma once

enum class Platform {
#ifdef _CONSOLE
	Console,
#endif
	SDL3
};

#ifdef _CONSOLE
#define DEFAULT_PLATFORM Platform::Console
#else
#define DEFAULT_PLATFORM Platform::SDL3
#endif
