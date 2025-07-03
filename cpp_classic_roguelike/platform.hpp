// local includes
#include "console.hpp"

// std includes
#include<memory>

std::shared_ptr<Console> make_platform_console(char width, char height);