// local imports
import console;
import constants;
#include "game.hpp"
#include "init_menu.hpp"
import mob;
import player;
import tile_map;

// std imports
import <memory>;
import <random>;
import <vector>;


int main(void)
{
	if (!init_menu()) return 0;

	Console::instance().clear();

	//console->set_full_screen(true);
	//console->hide_cursor();

	std::random_device random_device;
	std::default_random_engine random_device_impl(random_device());
	auto rand_x_pos = std::uniform_int_distribution<int>(0, (MAP_WIDTH - 1));
	auto rand_y_pos = std::uniform_int_distribution<int>(0, (MAP_HEIGHT - 1));
	std::vector<std::shared_ptr<Mob>> mobs;

	TileMap test(MAP_WIDTH, MAP_HEIGHT);

	std::shared_ptr<Mob> player = initial_player_spawn(mobs);

	initial_mobs_spawn(mobs, random_device_impl, rand_x_pos, rand_y_pos);
	
	while (player->is_alive())
	{
		Console::instance().clear();
		Console::instance().write(test);
		for (auto& mob : mobs)
		{
			Console::instance().write(mob->get_glyph(), mob->get_position());
		}

		Console::instance().present();

		advance_time(mobs);
		update_mobs(mobs, test);
		clean_mobs(mobs);
	}

	return 0;
}

