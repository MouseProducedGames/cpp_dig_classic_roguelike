// local imports
import base_map;
import console;
import constants;
import glyph;
import key_event;
#include "platform.hpp"
import player;
import tile_displacement;
import tile_map;

// std imports
import <chrono>;
import <memory>;
import <print>;
import <ranges>;
import <thread>;

int main(void)
{
	make_platform_console(MAP_WIDTH + 1, MAP_HEIGHT + 1);
	//console->set_full_screen(true);
	//console->hide_cursor();

	Player player('@', TilePosition(0, MAP_HEIGHT / 2));
	TileMap test(MAP_WIDTH, MAP_HEIGHT);
	while (true)
	{
		test.set_tile(player.get_position(), TileGlyphIndex::Floor);
		Console::instance().write(test);
		Console::instance().write(player.get_glyph(), player.get_position());

		Console::instance().present();

		auto key_maybe = Console::instance().read_key();
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

		if (test.size() != player.get_position())
		{
			break;
		}
		
		if (test[player.get_position()] == TileGlyphIndex::Wall)
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(250ms);
		}
	}

	return 0;
}
