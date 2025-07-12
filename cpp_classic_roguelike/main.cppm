// local imports
import console;
import constants;
#include "game.hpp"
#include "init_menu.hpp"
#include "level.hpp"
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

	Level level;

	auto player = initial_player_spawn(level);

	initial_mobs_spawn(level, random_device_impl, rand_x_pos, rand_y_pos);
	
	while (player->is_alive())
	{
		pre_update_mobs(level);
		//clean_mobs(level.get_mobs());
		sort_by_time(level.get_mobs());
		advance_time(level.get_mobs());

		Console::instance().clear();
		Console::instance().write(level.get_tile_map());
		Console::instance().write(level.get_mobs());

		Console::instance().present();

		update_mobs(level);
		//clean_mobs(level.get_mobs());
		sort_by_time(level.get_mobs());
		advance_time(level.get_mobs());
	}

	return 0;
}

