// local imports
import base_map;
import brains;
import console;
import constants;
import glyph;
import key_event;
import mob;
#include "make_platform_console.hpp"
import player;
import tile_displacement;
import tile_position;
import tile_map;

// std imports
import <algorithm>;
import <cctype>;
import <chrono>;
import <iostream>;
import <limits>;
import <memory>;
import <print>;
import <random>;
import <ranges>;
import <thread>;
import <vector>;

// No, I don't know why it has to be done like this.
struct {
	bool operator()(auto& lhs, auto& rhs)
	{
		return lhs->get_next_action_time() < rhs->get_next_action_time();
	}
} SortByNextActionTime;

char read_char();

void print(std::string s);

void println();

void println(std::string s);

int main(void)
{
	make_platform_console(MAP_WIDTH + 1, MAP_HEIGHT + 1);

	std::vector<char> from_index_chars = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	std::reverse(from_index_chars.begin(), from_index_chars.end());
	std::vector<char> index_chars;
	std::vector<Platform> index_platforms;
#ifdef _CONSOLE
	index_chars.push_back(from_index_chars.back());
	from_index_chars.pop_back();
	index_platforms.push_back(Platform::Console);
#endif // _CONSOLE

	index_chars.push_back(from_index_chars.back());
	from_index_chars.pop_back();
	index_platforms.push_back(Platform::SDL3);

	size_t index = 0;
#ifdef _CONSOLE
	println(std::format("{}) Classic terminal", index_chars[index++]));
#endif // _CONSOLE

	println(std::format("{}) SDL3 terminal", index_chars[index++]));
	println();
#ifndef _CONSOLE
	Console::instance().present();
#endif // !_CONSOLE

	char output = read_char();
	output = std::toupper(output);
	auto index_char_it = std::find(index_chars.begin(), index_chars.end(), output);
	if (index_char_it == index_chars.end()) return 0;

	size_t index_char = index_char_it - index_chars.begin();
	if (index_platforms[index_char] != DEFAULT_PLATFORM)
	{
		make_platform_console(
			MAP_WIDTH + 1,
			MAP_HEIGHT + 1,
			index_platforms[index_char]
		);
	}
	Console::instance().clear();

	//console->set_full_screen(true);
	//console->hide_cursor();

	std::random_device r;
	std::default_random_engine e1(r());
	auto rand_x_pos = std::uniform_int_distribution<int>(0, (MAP_WIDTH - 1));
	auto rand_y_pos = std::uniform_int_distribution<int>(0, (MAP_HEIGHT - 1));
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
		Console::instance().clear();
		//test.set_tile(player->get_position(), TileGlyphIndex::Floor);
		Console::instance().write(test);
		for (auto& mob : mobs)
		{
			Console::instance().write(mob->get_glyph(), mob->get_position());
		}

		Console::instance().present();

		
		{
			/*for (auto& mob : mobs)
			{
				auto cur_time = mob->get_next_action_time() - 1.0;
				mob->set_next_action_time(cur_time);
			}*/

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

			if (smallest_time > 0.0)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(50ms * smallest_time);
			}

			for (auto& mob : mobs)
			{
				auto cur_time = mob->get_next_action_time();
				mob->set_next_action_time(cur_time - smallest_time);
			}
		}

		std::sort(mobs.begin(), mobs.end(), SortByNextActionTime);

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
			std::shared_ptr<Mob> last = mobs[mobs.size() - 1];
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

char read_char()
{
#ifdef _CONSOLE
	std::println("Select an option and tap enter or return");
	std::println("Any other key to exit");
	char output = ' ';
	std::cin >> output;
	return output;
#else
	println("Select an option");
	println("Any other key to exit");
	return Console::instance().wait_key().key_char_union.ascii_char;
#endif // _CONSOLE
}

void print(std::string s)
{
#ifdef _CONSOLE
	std::print("{}", s);
#else
	Console::instance().write(s);
#endif // _CONSOLE
}

void println()
{
#ifdef _CONSOLE
	std::println();
#else
	Console::instance().write_line();
#endif // _CONSOLE
}

void println(std::string s)
{
#ifdef _CONSOLE
	std::println("{}", s);
#else
	Console::instance().write_line(s);
#endif // _CONSOLE
}
