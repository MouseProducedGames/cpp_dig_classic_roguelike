// this include
#include "win_console.hpp"

// local includes
#include "../../constants.hpp"

// std includes
#include<optional>
#include<print>
#include<stdint.h>

// Platform includes
#define WIN32_LEAN_AND_MEAN

#include<Windows.h>

#define MAX
#define MIN

WinConsole::WinConsole() : _back_buffer_index(0)
{
	_buffers[0].resize(MAP_TILE_COUNT, ' ');
	_buffers[1].resize(MAP_TILE_COUNT, ' ');
}

std::optional<KeyEvent> WinConsole::read_key()
{
	HANDLE stdinput = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD input;
	DWORD actually_read;
	if (ReadConsoleInput(stdinput, &input, (DWORD)1, (LPDWORD)(&actually_read)))
	{
		if (input.EventType == KEY_EVENT) {
			KEY_EVENT_RECORD key_event_record = *((KEY_EVENT_RECORD*)(&input.Event));
			if (key_event_record.bKeyDown)
			{
				KeyEvent output = { 0 };
				output.virtual_key_code = (VirtualKeyCode)(std::uint8_t)key_event_record.wVirtualKeyCode;
				output.virtual_scan_code = (VirtualScanCode)(std::uint8_t)key_event_record.wVirtualScanCode;
				output.key_char_union.unicode_char = key_event_record.uChar.UnicodeChar;
				return output;
			}
		}
	}
	
	return std::nullopt;
}

void WinConsole::present()
{
	std::size_t index = 0;
	for (std::size_t y = 0; y < 40; ++y)
	{
		move_hardware_cursor(0, static_cast<char>(y));
		for (std::size_t x = 0; x < 80; ++x)
		{
			char ch = _buffers[_back_buffer_index][index];
			if (ch != _buffers[_fore_buffer_index()][index])
			{
				_buffers[_fore_buffer_index()][index] = ch;
				move_hardware_cursor(static_cast<char>(x), static_cast<char>(y));
				std::print("{}", ch);
			}

			index += 1;
		}
	}

	_back_buffer_index = _fore_buffer_index();
}

void WinConsole::write(char ch)
{
	_buffers[_back_buffer_index][_cursor_tile_index()] = ch;
	//_buffers[_fore_buffer_index()][_cursor_tile_index()] = ch;
}

void WinConsole::write(char ch, char x, char y)
{
	move_cursor(x, y);
	write(ch);
}

std::size_t WinConsole::_fore_buffer_index()
{
	return 1 - _back_buffer_index;
}

std::size_t WinConsole::_cursor_tile_index()
{
	return (((std::size_t)_cursor_y) * MAP_WIDTH) + ((std::size_t)_cursor_x);
}