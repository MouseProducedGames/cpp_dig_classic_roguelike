// local includes
#include "tags.hpp"
#include "platform.hpp"

#pragma warning( push )
#pragma warning( disable : 5201 ) // Until they fix mixing includes and imports.
export module console;
#pragma warning( pop )

// local imports
import base_map;
import constants;
import glyph;
import key_event;
import mob;
import sizei;
import tile_position;

// std imports
import <cctype>;
import <chrono>;
import <memory>;
import <optional>;
import <print>;
import <thread>;
import <vector>;

export class Console
{
public:
	Console()
	{
#ifdef _CONSOLE
		std::print("\x1b[?1049h");
#endif
		hide_cursor();
		clear();
		move_cursor(0, 0);
	}
	virtual ~Console()
	{
#ifdef _CONSOLE
		std::print("\x1b[?1049l");
#endif
		show_cursor();
	}

	virtual void clear()
	{
//#ifdef _CONSOLE
//		std::print("\x1b[2J");
//#endif
	}

	/*virtual Sizei get_console_size() const = 0;*/

	virtual void hide_cursor()
	{
#ifdef _CONSOLE
		std::print("\x1b[?25l");
#endif
	}

	static Console& instance()
	{
		return *_INSTANCE;
	}

	bool move_cursor(TilePosition pos) { return move_cursor(pos.x, pos.y); }
	virtual bool move_cursor(char x, char y)
	{
		//printf("\033[%d;%dH", (y), (x));
		//std::print("\033[{};{}H", ((unsigned int)y), ((unsigned int)x));
		if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return false;

		_cursor_x = x;
		_cursor_y = y;

		return true;
	}

	virtual void present() = 0;

	virtual std::optional<KeyEvent> read_key() = 0;

	virtual void set_console_height(char height) = 0;
	void set_console_size(TilePosition pos) { set_console_size(pos.x, pos.y); }
	virtual void set_console_size(char width, char height)
	{
		set_console_height(height);
		set_console_width(width);
	}
	virtual void set_console_width(char width) = 0;

	virtual void set_full_screen(bool on) = 0;

	virtual void show_cursor()
	{
#ifdef _CONSOLE
		std::print("\x1b[?25h");
#endif
	}

	virtual KeyEvent wait_key()
	{
		while (true)
		{
			auto key_maybe = read_key();
			if (key_maybe.has_value())
			{
				return key_maybe.value();
			}

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}

	virtual void write(BaseMap<TileGlyphIndex>& map) = 0;
	virtual void write(char ch) = 0;
	virtual void write(std::vector<std::shared_ptr<Mob>>& mobs)
	{
		for (auto& mob : mobs)
		{
			if (mob->is_dead()) continue;

			if (!move_cursor(mob->get_position())) continue;
			char ch;
			if (mob->has_tag(TAG_PLAYER)) ch = '@';
			else ch = 'w';
			if (mob->has_tag(TAG_LARGE)) ch = std::toupper(ch);
			write(ch);
		}
	}
	virtual void write(std::string s)
	{
		write(s, _cursor_x, _cursor_y);
	}
	virtual void write(char ch, TilePosition pos) { write(ch, pos.x, pos.y); }
	virtual void write(char ch, char x, char y) = 0;
	virtual void write(std::string s, TilePosition pos)
	{
		write(std::move(s), pos.x, pos.y);
	}
	virtual void write(std::string s, char x, char y) = 0;
	virtual void write_line()
	{
		move_cursor(_cursor_x, _cursor_y + 1);
	}
	virtual void write_line(std::string s)
	{
		write_line(s, _cursor_x, _cursor_y);
	}
	virtual void write_line(std::string s, TilePosition pos)
	{
		write(std::move(s), pos.x, pos.y);
	}
	virtual void write_line(std::string s, char x, char y) = 0;

protected:
	char _cursor_x = 0, _cursor_y = 0;

	void move_hardware_cursor(char x, char y)
	{
#ifdef _CONSOLE
		//std::print("\033[{};{}H", ((unsigned int)y), ((unsigned int)x));
		std::print(
			"\x1b[{};{}H",
			((unsigned int)y + 1),
			((unsigned int)x + 1)
		);
#endif
	}

private:
	static std::shared_ptr<Console> _INSTANCE;

	friend void make_platform_console(char width, char height, Platform platform);
};

std::shared_ptr<Console> Console::_INSTANCE;