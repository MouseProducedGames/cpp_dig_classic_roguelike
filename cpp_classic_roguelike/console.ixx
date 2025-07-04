export module console;

// local imports
import base_map;
import constants;
import glyph;
import key_event;
import sizei;
import tile_position;

// std imports
import <optional>;
import <print>;

export class Console
{
public:
	Console()
	{
		std::print("\x1b[?1049h");
		hide_cursor();
		clear();
		move_cursor(0, 0);
	}
	virtual ~Console()
	{
		std::print("\x1b[?1049l");
		show_cursor();
	}

	void clear()
	{
		std::print("\x1b[2J");
	}

	/*virtual Sizei get_console_size() const = 0;*/

	void hide_cursor()
	{
		std::print("\x1b[?25l");
	}

	void move_cursor(TilePosition pos) { move_cursor(pos.x, pos.y); }
	void move_cursor(char x, char y)
	{
		//printf("\033[%d;%dH", (y), (x));
		//std::print("\033[{};{}H", ((unsigned int)y), ((unsigned int)x));
		if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;

		_cursor_x = x;
		_cursor_y = y;
	}

	virtual void present() = 0;

	virtual std::optional<KeyEvent> read_key() = 0;

	virtual void set_console_height(char height) = 0;
	void set_console_size(TilePosition pos) { set_console_size(pos.x, pos.y); }
	void set_console_size(char width, char height)
	{
		set_console_height(height);
		set_console_width(width);
	}
	virtual void set_console_width(char width) = 0;

	virtual void set_full_screen(bool on) = 0;

	void show_cursor()
	{
		std::print("\x1b[?25h");
	}

	virtual void write(BaseMap<TileGlyphIndex>& map) = 0;
	virtual void write(char ch) = 0;
	virtual void write(char ch, TilePosition pos) { write(ch, pos.x, pos.y); }
	virtual void write(char ch, char x, char y) = 0;

protected:
	char _cursor_x = 0, _cursor_y = 0;

	void move_hardware_cursor(char x, char y)
	{
		//std::print("\033[{};{}H", ((unsigned int)y), ((unsigned int)x));
		std::print(
			"\x1b[{};{}H",
			((unsigned int)y + 1),
			((unsigned int)x + 1)
		);
	}
};
