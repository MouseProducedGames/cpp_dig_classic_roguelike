#pragma once

// std imports
#include<optional>

// local imports
import key_event;
import sizei;

class Console
{
public:
	Console();
	virtual ~Console();

	void clear();

	/*virtual Sizei get_console_size() const = 0;*/

	void hide_cursor();

	void move_cursor(char x, char y);

	virtual void present() = 0;

	virtual std::optional<KeyEvent> read_key() = 0;

	virtual void set_console_height(char height) = 0;
	void set_console_size(char width, char height);
	virtual void set_console_width(char width) = 0;

	virtual void set_full_screen(bool on) = 0;

	void show_cursor();

	virtual void write(char ch) = 0;
	virtual void write(char ch, char x, char y) = 0;

protected:
	char _cursor_x = 0, _cursor_y = 0;

	void move_hardware_cursor(char x, char y);
};