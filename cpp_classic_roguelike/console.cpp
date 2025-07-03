// this include
#include "console.hpp"

// local includes
#include "constants.hpp"

// std includes
#include<print>


#define      with ";"
#define     plain "0" /* or "" */
#define        no "2"
#define    bright "1"
#define       dim "2"
#define    italic "3"
#define underline "4"
#define   reverse "7"

#define        fg "3"
#define        bg "4"
#define     br_fg "9"
#define     br_bg "10"
#define     black "0"
#define       red "1"
#define     green "2"
#define    yellow "3"
#define      blue "4"
#define   magenta "5"
#define      cyan "6"
#define     white "7"

#define    alt_buf "?1049"
#define       curs "?25"
#define term_clear "2J"
#define clear_line "2K"
#define       high "h"
#define        low "l"
#define       jump "H"

#define esc "\x1b"
#define esca esc "["
#define wfg "38;5;"
#define wbg "48;5;"
#define color "m"
#define fmt(f) esca f "m"

#define say(s) write(1,s,sizeof(s))
#define sz(s) (sizeof(s)/sizeof(*s))

Console::Console()
{
	std::print("\x1b[?1049h");
	hide_cursor();
	clear();
	move_cursor(0, 0);
}

Console::~Console()
{
	std::print("\x1b[?1049l");
	show_cursor();
}

void Console::clear()
{
	std::print("\x1b[2J");
}

void Console::hide_cursor()
{
	std::print("\x1b[?25l");
}

void Console::move_cursor(char x, char y)
{
	//printf("\033[%d;%dH", (y), (x));
	//std::print("\033[{};{}H", ((unsigned int)y), ((unsigned int)x));
	if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;
			
	_cursor_x = x;
	_cursor_y = y;
}

void Console::set_console_size(char width, char height)
{
	set_console_height(height);
	set_console_width(width);
}

void Console::show_cursor()
{
	std::print("\x1b[?25h");
}

void Console::move_hardware_cursor(char x, char y)
{
	//std::print("\033[{};{}H", ((unsigned int)y), ((unsigned int)x));
	std::print("\x1b[{};{}H", ((unsigned int)y + 1), ((unsigned int)x + 1));
}