// local imports
import base_map;
import console;
import constants;
import glyph;
import key_event;
import mob;
#include "platform.hpp"
import player;
import tile_displacement;
import tile_position;
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

	Mob player(
		'@',
		TilePosition(0, MAP_HEIGHT / 2),
		std::unique_ptr<MobBrain>(new PlayerBrain())
	);
	TileMap test(MAP_WIDTH, MAP_HEIGHT);
	while (true)
	{
		test.set_tile(player.get_position(), TileGlyphIndex::Floor);
		Console::instance().write(test);
		Console::instance().write(player.get_glyph(), player.get_position());

		Console::instance().present();

		player.update();

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
