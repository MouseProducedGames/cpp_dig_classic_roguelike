// local imports
import base_map;
import brains;
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
import <limits>;
import <memory>;
import <print>;
import <random>;
import <ranges>;
import <thread>;
import <vector>;

int main(void)
{
	std::random_device r;
	std::default_random_engine e1(r());
	auto rand_x_pos = std::uniform_int_distribution<int>(0, (MAP_WIDTH - 1));
	auto rand_y_pos = std::uniform_int_distribution<int>(0, (MAP_HEIGHT - 1));
	make_platform_console(MAP_WIDTH + 1, MAP_HEIGHT + 1);
	//console->set_full_screen(true);
	//console->hide_cursor();

	std::vector<std::shared_ptr<Mob>> mobs;

	mobs.push_back(std::make_shared<Mob>(
		'@',
		TilePosition(0, MAP_HEIGHT / 2),
		std::unique_ptr<MobBrain>(new PlayerBrain())
	));

	std::shared_ptr<Mob> player = mobs[0];


	for (int i = 1; i++ < 20;)
	{
		mobs.push_back(std::make_shared<Mob>(
			'W',
			TilePosition(rand_x_pos(e1), rand_y_pos(e1)),
			std::unique_ptr<MobBrain>(new RandomMoveBrain())
		));
	}

	std::vector<size_t> removes;
	TileMap test(MAP_WIDTH, MAP_HEIGHT);
	while (true)
	{
		//test.set_tile(player->get_position(), TileGlyphIndex::Floor);
		Console::instance().write(test);
		for (auto& mob : mobs)
		{
			Console::instance().write(mob->get_glyph(), mob->get_position());
		}

		Console::instance().present();

		
		{
			for (auto& mob : mobs)
			{
				auto cur_time = mob->get_next_action_time() - 1.0;
				mob->set_next_action_time(cur_time);
			}

			// Zero-centering initiative time to avoid drift.
			double smallest_time = std::numeric_limits<double>::max();
			for (auto& mob : mobs)
			{
				auto cur_time = mob->get_next_action_time();
				if (cur_time < smallest_time)
				{
					smallest_time = cur_time;
				}
			}

			for (auto& mob : mobs)
			{
				auto cur_time = mob->get_next_action_time();
				mob->set_next_action_time(cur_time - smallest_time);
			}
		}

		std::sort(
			mobs.begin(),
			mobs.end(),
			[](auto& lhs, auto& rhs)
			{
				return lhs->get_next_action_time() < rhs->get_next_action_time();
			}
		);

		for (auto& mob : mobs)
		{
			if (mob->get_next_action_time() > 0.0001) break;

			test.set_tile(mob->get_position(), TileGlyphIndex::Floor);

			if (mob->is_dead()) continue;

			mob->update(test);

			if (mob->is_dead()) continue;

			for (auto& other_mob : mobs)
			{
				if (mob == other_mob) continue;

				if (mob->get_position() == other_mob->get_position())
				{
					other_mob->kill();
				}
			}
		}

		for (auto [index, mob] : std::views::enumerate(mobs))
		{
			if (mob->is_dead())
			{
				removes.push_back(index);
			}
		}

		if (player->is_dead())
		{
			break;
		}

		for (auto index : std::views::reverse(removes))
		{
			auto last = mobs[mobs.size() - 1];
			mobs[mobs.size() - 1] = mobs[index];
			mobs[index] = last;
			mobs.pop_back();
		}
		removes.clear();
		
		/*if (test[player->get_position()] == TileGlyphIndex::Wall)
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(250ms);
		}*/
	}

	return 0;
}
