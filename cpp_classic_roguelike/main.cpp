// local headers
#include "base_map.hpp"
#include "console.hpp"
#include "platform.hpp"
#include "tile_map.hpp"

// std headers
#include<memory>
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
			//std::print("{}", (unsigned int)key_maybe.value());
			switch (key_maybe.value())
			{
			case 71:
			case 16: player_x -= 1; player_y -= 1; break;
			case 72:
			case 17: player_y -= 1; break;
			case 73:
			case 18: player_x += 1; player_y -= 1; break;
			case 75:
			case 30: player_x -= 1; break;
			case 76:
			case 31: break;
			case 77:
			case 32: player_x += 1; break;
			case 79:
			case 44: player_x -= 1; player_y += 1; break;
			case 80:
			case 45: player_y += 1; break;
			case 81:
			case 46: player_x += 1; player_y += 1; break;
			}
		}

		if (player_x > test.width() || player_y > test.height())
		{
			break;
		}
	}

	return 0;
}