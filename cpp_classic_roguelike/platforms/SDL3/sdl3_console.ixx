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
import <algorithm>;
import <array>;
import <exception>;
import <optional>;
import <print>;
import <stdint.h>;
import <vector>;

// platform imports
//import windows;

static std::array<Glyph, 2> TILE_GLYPHS = { '#', '.' };

static float _text_scale = 8.0f;

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

		_window = SDL_CreateWindow("CPP Classic Roguelike", 800, 600, 0);
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

		SDL_DisplayMode const* display_mode =
		//SDL_GetCurrentDisplayMode(0, &display_mode);
		SDL_GetDesktopDisplayMode(1);
		if (display_mode)
		{
			float fwts = static_cast<float>(width) * _text_scale;
			float fhts = static_cast<float>(height) * _text_scale;
			_scale = static_cast<float>(std::min(
				static_cast<float>(display_mode->w) / fwts,
				static_cast<float>(display_mode->h) / fhts
			));
		}
		else {
			std::println("SDL_GetError(): {}", SDL_GetError());
			_scale = 1.0;
		}
		set_console_size(width, height);
		SDL_SetWindowFullscreen(_window, true);
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

	virtual void clear()
	{
		SDL_SetRenderScale(_renderer, 1.0, 1.0);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);

		std::size_t index = 0;
		for (std::size_t y = 0; y < MAP_HEIGHT; ++y)
		{
			move_hardware_cursor(0, static_cast<char>(y));
			for (std::size_t x = 0; x < MAP_WIDTH; ++x)
			{
				_buffers[_fore_buffer_index()][index] = ' ';

				index += 1;
			}
		}
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
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		SDL_SetRenderScale(_renderer, _scale, _scale);
		std::size_t index = 0;
		std::array<char, 2> chars = { ' ', '\0' };
		for (std::size_t y = 0; y < MAP_HEIGHT; ++y)
		{
			move_hardware_cursor(0, static_cast<char>(y));
			for (std::size_t x = 0; x < MAP_WIDTH; ++x)
			{
				char ch = _buffers[_back_buffer_index][index];
				if (ch != _buffers[_fore_buffer_index()][index])
				{
					_buffers[_fore_buffer_index()][index] = ch;
					chars[0] = ch;
					SDL_RenderDebugText(
						_renderer,
						static_cast<float>(x) * _text_scale,
						static_cast<float>(y) * _text_scale,
						&chars[0]
					);
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
		int w = 0, h = 0;
		SDL_GetWindowSize(_window, &w, &h);
		h = static_cast<int>(static_cast<float>(height) * _text_scale * _scale);
		SDL_SetWindowSize(_window, w, h);
	}
	virtual void set_console_width(char width)
	{
		int w = 0, h = 0;
		SDL_GetWindowSize(_window, &w, &h);
		w = static_cast<int>(static_cast<float>(width) * _text_scale * _scale);
		SDL_SetWindowSize(_window, w, h);
	}

	virtual void set_full_screen(bool on)
	{
		SDL_SetWindowFullscreen(_window, on);
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

	float _scale;
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
