// this include
#include "init_menu.hpp"

// local imports
import console;
import constants;
#include "make_platform_console.hpp"

// std imports
import <format>;
import <string>;
import <vector>;

char read_char();

void print(std::string s);
void println();
void println(std::string s);

bool init_menu()
{
	make_platform_console(MAP_WIDTH + 1, MAP_HEIGHT + 1);

	std::vector<char> from_index_chars = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	std::reverse(from_index_chars.begin(), from_index_chars.end());
	std::vector<char> index_chars;
	std::vector<Platform> index_platforms;
#ifdef _CONSOLE
	index_chars.push_back(from_index_chars.back());
	from_index_chars.pop_back();
	index_platforms.push_back(Platform::Console);
#endif // _CONSOLE

	index_chars.push_back(from_index_chars.back());
	from_index_chars.pop_back();
	index_platforms.push_back(Platform::SDL3);

	size_t index = 0;
#ifdef _CONSOLE
	println(std::format("{}) Classic terminal", index_chars[index++]));
#endif // _CONSOLE

	println(std::format("{}) SDL3 terminal", index_chars[index++]));
	println();

	char output = read_char();
	output = std::toupper(output);
	auto index_char_it = std::find(index_chars.begin(), index_chars.end(), output);
	if (index_char_it == index_chars.end()) return false;

	size_t index_char = index_char_it - index_chars.begin();
	if (index_platforms[index_char] != DEFAULT_PLATFORM)
	{
		make_platform_console(
			MAP_WIDTH + 1,
			MAP_HEIGHT + 1,
			index_platforms[index_char]
		);
	}

	return true;
}


char read_char()
{
	println("Select an option");
	println("Any other key to exit");
	Console::instance().present();
	return Console::instance().wait_key().key_char_union.ascii_char;
}

void print(std::string s)
{
	Console::instance().write(s);
}
void println()
{
	Console::instance().write_line();
}
void println(std::string s)
{
	Console::instance().write_line(s);
}
