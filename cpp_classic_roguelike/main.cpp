// local headers
#include "base_map.hpp"
#include "console.hpp"
#include "key_event.hpp"
#include "platform.hpp"
#include "tile_map.hpp"

// std headers
#include<memory>
#include<print>
#include<ranges>

int main(void)
{
	auto console = make_platform_console();

	size_t player_x = 8, player_y = 5;
	while (true)
	{
		//console->clear();
		
		TileMap test(80, 40);
		for (size_t y = 0; y < test.height(); ++y)
		{
			//console->move_cursor(0, static_cast<char>(y));
			//std::print("y: {}", y);
			for (size_t x = 0; x < test.width(); ++x)
			{
				/*auto tile = test[x, y];
				std::print("{}", tile);*/
				console->write(test.get_tile(x, y).value(), x, y);
				//auto tile_maybe = test.get_tile(x, y);
				//if (tile_maybe.has_value())
				//{
				//	//std::print("{}", );
				//	std::print("{}", tile_maybe.value());
				//}
			}
		}

		console->write('@', player_x, player_y);

		console->present();

		auto key_maybe = console->read_key();
		if (key_maybe.has_value())
		{
			//std::print("{}", (unsigned int)key_maybe.value().virtual_scan_code);
			switch (key_maybe.value().virtual_scan_code)
			{
			case VirtualScanCode::Q:
			case VirtualScanCode::Numpad7: player_x -= 1; player_y -= 1; break;
			case VirtualScanCode::W:
			case VirtualScanCode::Numpad8: player_y -= 1; break;
			case VirtualScanCode::E:
			case VirtualScanCode::Numpad9: player_x += 1; player_y -= 1; break;
			case VirtualScanCode::A:
			case VirtualScanCode::Numpad4: player_x -= 1; break;
			case VirtualScanCode::S:
			case VirtualScanCode::Numpad5: break;
			case VirtualScanCode::D:
			case VirtualScanCode::Numpad6: player_x += 1; break;
			case VirtualScanCode::Z:
			case VirtualScanCode::Numpad1: player_x -= 1; player_y += 1; break;
			case VirtualScanCode::X:
			case VirtualScanCode::Numpad2: player_y += 1; break;
			case VirtualScanCode::C:
			case VirtualScanCode::Numpad3: player_x += 1; player_y += 1; break;
			}
		}

		if (player_x > test.width() || player_y > test.height())
		{
			break;
		}
	}

	return 0;
}