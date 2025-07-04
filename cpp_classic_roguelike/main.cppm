// local headers
import base_map;
#include "console.hpp"
#include "constants.hpp"
import key_event;
#include "platform.hpp";
import player;
import tile_displacement;
import tile_map;

// std headers
#include<chrono>
#include<memory>
#include<print>
#include<ranges>
#include<thread>
//import <thread>

int main(void)
{
	auto console = make_platform_console(MAP_WIDTH, MAP_HEIGHT);
	//console->set_full_screen(true);
	//console->hide_cursor();

	Player player('@', TilePosition(0, 20));
	TileMap test(MAP_WIDTH, MAP_HEIGHT);
	while (true)
	{
		test.set_tile(player.get_position().x, player.get_position().y, '.');
		//console->clear();
		for (size_t y = 0; y < test.height(); ++y)
		{
			for (size_t x = 0; x < test.width(); ++x)
			{
				console->write(
					test.get_tile(x, y).value(),
					static_cast<char>(x),
					static_cast<char>(y)
				);
			}
		}
		console->write(player.get_glyph(), player.get_position().x, player.get_position().y);

		console->present();

		auto key_maybe = console->read_key();
		if (key_maybe.has_value())
		{
			//std::print("{}", (unsigned int)key_maybe.value().virtual_scan_code);
			TileDisplacement move;
			switch (key_maybe.value().virtual_scan_code)
			{
			case VirtualScanCode::Q:
			case VirtualScanCode::Numpad7: move.x = -1; move.y = -1; break;
			case VirtualScanCode::W:
			case VirtualScanCode::Numpad8: move.y = -1; break;
			case VirtualScanCode::E:
			case VirtualScanCode::Numpad9: move.x = 1; move.y = -1; break;
			case VirtualScanCode::A:
			case VirtualScanCode::Numpad4: move.x = -1; break;
			case VirtualScanCode::S:
			case VirtualScanCode::Numpad5: break;
			case VirtualScanCode::D:
			case VirtualScanCode::Numpad6: move.x = 1; break;
			case VirtualScanCode::Z:
			case VirtualScanCode::Numpad1: move.x = -1; move.y = 1; break;
			case VirtualScanCode::X:
			case VirtualScanCode::Numpad2: move.y = 1; break;
			case VirtualScanCode::C:
			case VirtualScanCode::Numpad3: move.x = 1; move.y = 1; break;
			}

			player.move(move);
		}

		if (player.get_position().x >= test.width() || player.get_position().y >= test.height())
		{
			break;
		}
		
		if (test.get_tile(player.get_position().x, player.get_position().y).value() == '#')
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(250ms);
		}
	}

	return 0;
}