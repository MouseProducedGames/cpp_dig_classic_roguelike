#pragma once

// local imports
import tile_map;
import tile_position;

// std imports
import <generator>;
import <memory>;
import <optional>;
import <utility>;
import <vector>;

class Mob;

class Level
{
public:
	Level();

	void add_mob(std::shared_ptr<Mob> mob) noexcept;

	/*std::optional<std::shared_ptr<Mob>> get_mob_at(char x, char y) const noexcept;
	std::optional<std::shared_ptr<Mob>> get_mob_at(TilePosition tile_position) const noexcept;*/

	std::vector<std::shared_ptr<Mob>>& get_mobs() noexcept;
	std::vector<std::shared_ptr<Mob>> const& get_mobs() const noexcept;

	TileMap& get_tile_map() noexcept;
	TileMap const& get_tile_map() const noexcept;

	std::generator<std::shared_ptr<Mob>> iter_mobs() const noexcept;
	std::generator<std::shared_ptr<Mob>> iter_mobs_at(
		char x,
		char y
	) const noexcept;
	std::generator<std::shared_ptr<Mob>> iter_mobs_at(
		TilePosition tile_position
	) const noexcept;

	template<typename... TArgs> void new_mob(TArgs&&... args)
	{
		_mobs.emplace_back(new Mob(std::forward<TArgs>(args)...));
	}

private:
	std::vector<std::shared_ptr<Mob>> _mobs;
	TileMap _tile_map;
};
