#include "level.hpp"

// local imports
import constants;
import mob;
import tile_map;
import tile_position;

// std imports
import <generator>;
import <memory>;
import <optional>;
import <vector>;

Level::Level() : _tile_map(MAP_WIDTH, MAP_HEIGHT) {}

void Level::add_mob(std::shared_ptr<Mob> mob) noexcept
{
	_mobs.push_back(mob);
}

//std::optional<std::shared_ptr<Mob>> Level::get_mob_at(TilePosition tile_position) const noexcept
//{
//	for (auto& mob : _mobs)
//	{
//		if (mob->get_position() == tile_position)
//		{
//			return mob;
//		}
//	}
//
//	return std::nullopt;
//}
//std::optional<std::shared_ptr<Mob>> Level::get_mob_at(char x, char y) const noexcept
//{
//	return get_mob_at(TilePosition(x, y));
//}

std::vector<std::shared_ptr<Mob>>& Level::get_mobs() noexcept
{
	return _mobs;
}
std::vector<std::shared_ptr<Mob>> const& Level::get_mobs() const noexcept
{
	return _mobs;
}

TileMap& Level::get_tile_map() noexcept
{
	return _tile_map;
}
TileMap const& Level::get_tile_map() const noexcept
{
	return _tile_map;
}

std::generator<std::shared_ptr<Mob>> Level::iter_mobs() const noexcept
{
	for (auto mob : _mobs)
	{
		co_yield mob;
	}
}

std::generator<std::shared_ptr<Mob>> Level::iter_mobs_at(
	char x,
	char y
) const noexcept
{
	return iter_mobs_at(TilePosition(x, y));
}
std::generator<std::shared_ptr<Mob>> Level::iter_mobs_at(
	TilePosition tile_position
) const noexcept
{
	for (auto& mob : _mobs)
	{
		if (mob->get_position() == tile_position)
			co_yield mob;
	}
}