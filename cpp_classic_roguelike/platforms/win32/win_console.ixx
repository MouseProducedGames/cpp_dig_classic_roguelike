// Platform includes
#define WIN32_LEAN_AND_MEAN
//#define WINVER 0x0500

#include<Windows.h>

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module win_console;
#pragma warning( pop )

#define MAX
#define MIN

// local imports
import console;
import constants;
import sizei;

// std imports
import <array>;
import <optional>;
import <print>;
import <stdint.h>;
import <vector>;

// platform imports
//import windows;

export class WinConsole : public Console
{
public:
	WinConsole(char width, char height) : Console()
	{
		set_console_size(width, height);
		_back_buffer_index = 0;
		_buffers[0].resize(MAP_TILE_COUNT, ' ');
		_buffers[1].resize(MAP_TILE_COUNT, ' ');
	}
	virtual ~WinConsole() = default;

	/*virtual Sizei get_console_size() const;*/

	virtual std::optional<KeyEvent> read_key()
	{
		HANDLE stdinput = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD input;
		DWORD actually_read = 0;
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

	virtual void present()
	{
		std::size_t index = 0;
		for (std::size_t y = 0; y < MAP_HEIGHT; ++y)
		{
			move_hardware_cursor(0, static_cast<char>(y));
			for (std::size_t x = 0; x < MAP_WIDTH; ++x)
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

	virtual void set_console_height(char height)
	{
		auto blah = GetSystemMetrics(SM_CYMIN);

		auto stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFOEX buffer_info = {};
		buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(stdoutput, &buffer_info);
		buffer_info.dwSize.Y = (height + 2); // Just slightly...
		buffer_info.srWindow.Bottom = buffer_info.srWindow.Top + (height + 1);
		/*SMALL_RECT screen = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(stdoutput, true, &screen);*/
		if ((height + 1) > buffer_info.dwMaximumWindowSize.Y)
			buffer_info.dwMaximumWindowSize.Y = (height + 1);
		if (!SetConsoleScreenBufferInfoEx(stdoutput, &buffer_info))
			std::print("    SetConsoleScreenBufferInfoEx error: {}", GetLastError());
	}
	virtual void set_console_width(char width)
	{
		auto blah = GetSystemMetrics(SM_CXMIN);

		auto stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFOEX buffer_info = {};
		buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(stdoutput, &buffer_info);
		buffer_info.dwSize.X = (width - 0); // ...bugged.
		buffer_info.srWindow.Right = buffer_info.srWindow.Left + (width - 1);
		if ((width - 1) > buffer_info.dwMaximumWindowSize.X)
			buffer_info.dwMaximumWindowSize.X = (width - 1);
		if (!SetConsoleScreenBufferInfoEx(stdoutput, &buffer_info))
			std::print("    SetConsoleScreenBufferInfoEx error: {}", GetLastError());
	}

	virtual void set_full_screen(bool on)
	{
		/*if (!SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN, 0))
			std::print("    SetConsoleDisplayMode error: {}", GetLastError());*/

		INPUT ip_alt_press = {};
		ip_alt_press.type = INPUT_KEYBOARD;
		//ip_alt_press.ki.wScan = 0xA6;
		//ip_alt_press.ki.wScan = 0;
		ip_alt_press.ki.wScan = 0x26; // alt key
		ip_alt_press.ki.time = 0;
		ip_alt_press.ki.dwExtraInfo = 0;
		ip_alt_press.ki.wVk = 0xA4; // alt key
		//ip_alt_press.ki.wVk = 0;
		ip_alt_press.ki.dwFlags = 0;

		INPUT ip_alt_release = ip_alt_press;
		ip_alt_release.ki.dwFlags = KEYEVENTF_KEYUP;

		/*if (!SendInput(1, &ip_alt, sizeof(INPUT)))
			std::print("    SendInputerror: {}", GetLastError());*/

		INPUT ip_enter_press = {};
		ip_enter_press.type = INPUT_KEYBOARD;
		//ip_enter_press.ki.wScan = 0;
		ip_enter_press.ki.wScan = 0x1C;
		ip_enter_press.ki.time = 200;
		ip_enter_press.ki.dwExtraInfo = 0;
		ip_enter_press.ki.wVk = 0xD; // enter key
		//ip_enter_press.ki.wVk = 0;
		ip_enter_press.ki.dwFlags = 0;

		INPUT ip_enter_release = ip_enter_press;
		ip_enter_release.ki.dwFlags = KEYEVENTF_KEYUP;

		std::array<INPUT, 4> inputs = { ip_alt_press, ip_enter_press, ip_enter_release, ip_alt_release };

		if (!SendInput(static_cast<UINT>(inputs.size()), &inputs[0], sizeof(INPUT)))
			std::print("    SendInputerror: {}", GetLastError());
	}


	virtual void write(BaseMap<Glyph>& map)
	{
		std::size_t index = 0;
		for (auto tile_pos : map.iter_tile_positions())
		{
			_buffers[_back_buffer_index][index++] = map[tile_pos];
		}
	}
	virtual void write(char ch)
	{
		_buffers[_back_buffer_index][_cursor_tile_index()] = ch;
		//_buffers[_fore_buffer_index()][_cursor_tile_index()] = ch;
	}
	virtual void write(char ch, char x, char y)
	{
		move_cursor(x, y);
		write(ch);
	}

protected:
	std::array<std::vector<char>, 2> _buffers;
	std::size_t _back_buffer_index;
	std::size_t _fore_buffer_index() const
	{
		return 1 - _back_buffer_index;
	}

	std::size_t _cursor_tile_index()
	{
		return (((std::size_t)_cursor_y) * MAP_WIDTH) + ((std::size_t)_cursor_x);
	}
};

//Sizei WinConsole::get_console_size() const
//{
//	HANDLE stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
//	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
//	GetConsoleScreenBufferInfo(stdoutput, &buffer_info);
//	
//	Sizei output(buffer_info.dwSize.X, buffer_info.dwSize.Y);
//	return output;
//}
