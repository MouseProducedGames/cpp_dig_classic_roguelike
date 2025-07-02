#pragma once

// local includes
#include "key_event.hpp"

// std includes
#include<optional>

class Console
{
public:
	Console();
	virtual ~Console();

	void clear();

	void hide_cursor();

	void move_cursor(char x, char y);

	virtual std::optional<KeyEvent> read_key() = 0;

	void show_cursor();

	virtual void present() = 0;

	virtual void write(char ch) = 0;
	virtual void write(char ch, char x, char y) = 0;

protected:
	char _cursor_x = 0, _cursor_y = 0;

	void move_hardware_cursor(char x, char y);
};