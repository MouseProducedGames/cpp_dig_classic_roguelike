// this include
#include "game.hpp"
#include "tags.hpp"

// local imports
import brains;
import constants;
import mob;
import player;

// std imports
import <chrono>;
import <memory>;
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

void advance_time(std::vector<std::shared_ptr<Mob>>& mobs)
{
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

	std::sort(mobs.begin(), mobs.end(), SortByNextActionTime);
}

void clean_mobs(std::vector<std::shared_ptr<Mob>>& mobs)
{
	static std::vector<size_t> removes;
	for (auto [index, mob] : std::views::enumerate(mobs))
	{
		if (mob->is_dead())
		{
			removes.push_back(index);
		}
	}

	for (auto index : std::views::reverse(removes))
	{
		std::shared_ptr<Mob> last = mobs[mobs.size() - 1];
		mobs[mobs.size() - 1] = mobs[index];
		mobs[index] = last;
		mobs.pop_back();
	}
	removes.clear();
}

void initial_mobs_spawn(
	std::vector<std::shared_ptr<Mob>>& mobs,
	std::default_random_engine& random_device_impl,
	std::uniform_int_distribution<int>& rand_x_pos,
	std::uniform_int_distribution<int>& rand_y_pos
)
{
	auto chance_large = std::uniform_int_distribution<int>(1, 100);
	for (int i = 1; i++ < 20;)
	{
		std::vector<TagName> tags;
		if (chance_large(random_device_impl) <= 5) tags.push_back(TAG_LARGE);
		mobs.push_back(std::shared_ptr<Mob>(new Mob(
			TilePosition(
				rand_x_pos(random_device_impl),
				rand_y_pos(random_device_impl)
			),
			std::unique_ptr<MobBrain>(new RandomMoveBrain()),
			tags
		)));
	}
}

std::shared_ptr<Mob> initial_player_spawn(std::vector<std::shared_ptr<Mob>>& mobs)
{
	std::shared_ptr<Mob> player = std::shared_ptr<Mob>(new Mob(
		TilePosition(0, MAP_HEIGHT / 2),
		std::unique_ptr<MobBrain>(new PlayerBrain()),
		{ TAG_PLAYER }
	));
	mobs.push_back(player);
	return player;
}

void update_mobs(std::vector<std::shared_ptr<Mob>>& mobs, TileMap& map)
{
	for (auto& mob : mobs)
	{
		if (mob->get_next_action_time() > 0.0001) break;

		map.set_tile(mob->get_position(), TileGlyphIndex::Floor);

		if (mob->is_dead()) continue;

		mob->update(map);

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
}