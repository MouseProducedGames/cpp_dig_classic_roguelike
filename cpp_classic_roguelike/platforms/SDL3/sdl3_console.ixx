#include "SDL3/SDL.h"

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module sdl3_console;
#pragma warning( pop )

// local imports
import console;
import constants;
import glyph;
import sizei;

// std imports
import <array>;
import <chrono>;
import <exception>;
import <optional>;
import <print>;
import <stdint.h>;
import <thread>;
import <vector>;

// platform imports
//import windows;

static std::array<Glyph, 2> TILE_GLYPHS = { '#', '.' };

export class SDL3Console : public Console
{
public:
	SDL3Console(char width, char height) : Console()
	{
		_back_buffer_index = 0;
		_buffers[0].resize(MAP_TILE_COUNT, ' ');
		_buffers[1].resize(MAP_TILE_COUNT, ' ');

		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				"Error initializing SDL3",
				nullptr
			);

			throw std::exception("Error initializing SDL3");
		}

		_window = SDL_CreateWindow("CPP Classic Roguelike", 1600, 800, 0);
		if (!_window)
		{
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				"Error creating window",
				nullptr
			);

			throw std::exception("Error creating window");
		}
		_renderer = SDL_CreateRenderer(_window, nullptr);
		if (!_renderer)
		{
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				"Error creating renderer",
				nullptr
			);

			throw std::exception("Error creating renderer");
		}
	}
	virtual ~SDL3Console()
	{
		if (_renderer)
		{
			SDL_DestroyRenderer(_renderer);
			_renderer = nullptr;;
		}
		if (_window)
		{
			SDL_DestroyWindow(_window);
			_window = nullptr;;
		}
		SDL_Quit();
	}

	/*virtual Sizei get_console_size() const;*/

	virtual std::optional<KeyEvent> read_key()
	{
		SDL_Event event = {};
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_KEY_DOWN) {
				SDL_KeyboardEvent key_event_record = *((SDL_KeyboardEvent*)(&event));
				KeyEvent output = { 0 };
				output.virtual_key_code =
					(VirtualKeyCode)(std::uint8_t)key_event_record.key;
				output.virtual_scan_code =
					(VirtualScanCode)(std::uint8_t)key_event_record.raw;
				output.key_char_union.unicode_char = SDL_GetKeyFromScancode(
					key_event_record.scancode,
					key_event_record.mod,
					true
					);
				return output;
			}
		}

		return std::nullopt;
	}

	virtual void present()
	{
		SDL_SetRenderScale(_renderer, 1.0, 1.0);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);

		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		SDL_SetRenderScale(_renderer, 2.0, 2.0);
		std::size_t index = 0;
		std::array<char, 2> chars = { ' ', '\0' };
		for (std::size_t y = 0; y < MAP_HEIGHT; ++y)
		{
			move_hardware_cursor(0, static_cast<char>(y));
			for (std::size_t x = 0; x < MAP_WIDTH; ++x)
			{
				char ch = _buffers[_back_buffer_index][index];
				//if (ch != _buffers[_fore_buffer_index()][index])
				{
					_buffers[_fore_buffer_index()][index] = ch;
					chars[0] = ch;
					SDL_RenderDebugText(_renderer, x * 10, y * 10, &chars[0]);
				}

				index += 1;
			}
		}

		_back_buffer_index = _fore_buffer_index();

		SDL_SetRenderScale(_renderer, 1.0, 1.0);
		SDL_RenderPresent(_renderer);
	}

	virtual void set_console_height(char height)
	{
		//auto blah = GetSystemMetrics(SM_CYMIN);

		//auto stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);

		//CONSOLE_SCREEN_BUFFER_INFOEX buffer_info = {};
		//buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		//GetConsoleScreenBufferInfoEx(stdoutput, &buffer_info);
		//buffer_info.dwSize.Y = (height + 2); // Just slightly...
		//buffer_info.srWindow.Bottom = buffer_info.srWindow.Top + (height + 1);
		///*SMALL_RECT screen = { 0, 0, 1, 1 };
		//SetConsoleWindowInfo(stdoutput, true, &screen);*/
		//if ((height + 1) > buffer_info.dwMaximumWindowSize.Y)
		//	buffer_info.dwMaximumWindowSize.Y = (height + 1);
		//if (!SetConsoleScreenBufferInfoEx(stdoutput, &buffer_info))
		//	std::print("    SetConsoleScreenBufferInfoEx error: {}", GetLastError());
	}
	virtual void set_console_width(char width)
	{
		//auto blah = GetSystemMetrics(SM_CXMIN);

		//auto stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);

		//CONSOLE_SCREEN_BUFFER_INFOEX buffer_info = {};
		//buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		//GetConsoleScreenBufferInfoEx(stdoutput, &buffer_info);
		//buffer_info.dwSize.X = (width - 0); // ...bugged.
		//buffer_info.srWindow.Right = buffer_info.srWindow.Left + (width - 1);
		//if ((width - 1) > buffer_info.dwMaximumWindowSize.X)
		//	buffer_info.dwMaximumWindowSize.X = (width - 1);
		//if (!SetConsoleScreenBufferInfoEx(stdoutput, &buffer_info))
		//	std::print("    SetConsoleScreenBufferInfoEx error: {}", GetLastError());
	}

	virtual void set_full_screen(bool on)
	{
		/*if (!SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN, 0))
			std::print("    SetConsoleDisplayMode error: {}", GetLastError());*/

		//INPUT ip_alt_press = {};
		//ip_alt_press.type = INPUT_KEYBOARD;
		////ip_alt_press.ki.wScan = 0xA6;
		////ip_alt_press.ki.wScan = 0;
		//ip_alt_press.ki.wScan = 0x26; // alt key
		//ip_alt_press.ki.time = 0;
		//ip_alt_press.ki.dwExtraInfo = 0;
		//ip_alt_press.ki.wVk = 0xA4; // alt key
		////ip_alt_press.ki.wVk = 0;
		//ip_alt_press.ki.dwFlags = 0;

		//INPUT ip_alt_release = ip_alt_press;
		//ip_alt_release.ki.dwFlags = KEYEVENTF_KEYUP;

		///*if (!SendInput(1, &ip_alt, sizeof(INPUT)))
		//	std::print("    SendInputerror: {}", GetLastError());*/

		//INPUT ip_enter_press = {};
		//ip_enter_press.type = INPUT_KEYBOARD;
		////ip_enter_press.ki.wScan = 0;
		//ip_enter_press.ki.wScan = 0x1C;
		//ip_enter_press.ki.time = 200;
		//ip_enter_press.ki.dwExtraInfo = 0;
		//ip_enter_press.ki.wVk = 0xD; // enter key
		////ip_enter_press.ki.wVk = 0;
		//ip_enter_press.ki.dwFlags = 0;

		//INPUT ip_enter_release = ip_enter_press;
		//ip_enter_release.ki.dwFlags = KEYEVENTF_KEYUP;

		//std::array<INPUT, 4> inputs = { ip_alt_press, ip_enter_press, ip_enter_release, ip_alt_release };

		//if (!SendInput(static_cast<UINT>(inputs.size()), &inputs[0], sizeof(INPUT)))
		//	std::print("    SendInputerror: {}", GetLastError());
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


	virtual void write(BaseMap<TileGlyphIndex>& map)
	{
		std::size_t index = 0;
		for (auto tile_pos : map.iter_tile_positions())
		{
			auto tile_index = static_cast<size_t>(map[tile_pos]);
			_ASSERT(tile_index <= TILE_GLYPHS.size());
			_buffers[_back_buffer_index][index++] = TILE_GLYPHS[tile_index];
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

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
};

//Sizei SDL3Console::get_console_size() const
//{
//	HANDLE stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
//	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
//	GetConsoleScreenBufferInfo(stdoutput, &buffer_info);
//	
//	Sizei output(buffer_info.dwSize.X, buffer_info.dwSize.Y);
//	return output;
//}
