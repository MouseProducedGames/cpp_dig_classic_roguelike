#include "interned_string.hpp"

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module mob;
#pragma warning( pop )

// local imports
import glyph;
import map_object;
import tile_map;
import tile_position;

// std imports
import <chrono>;
import <memory>;
import <random>;

class Mob;

export class MobBrain
{
public:
	MobBrain() = default;
	virtual ~MobBrain() = default;

	virtual void kill(Mob& mob) {}

	virtual void update(Mob& mob, TileMap& map) = 0;
};

export class Mob : public MapObject
{
public:
	// de/constructors
	Mob(std::unique_ptr<MobBrain> brain)
		: MapObject(), _brain(std::move(brain))
	{
		_default_random_engine =
			std::move(std::default_random_engine(_random_device()));
		_default_random_engine.seed(_rand_seed_counter++);

		auto d6 = std::uniform_int_distribution(1, 6);
		_mining_skill =
			d6(_default_random_engine) +
			d6(_default_random_engine) +
			d6(_default_random_engine);
	}
	Mob(
		std::unique_ptr<MobBrain> brain,
		std::initializer_list<InternedString> tags
	)
		: MapObject(tags), _brain(std::move(brain))
	{
		_default_random_engine =
			std::move(std::default_random_engine(_random_device()));
		_default_random_engine.seed(_rand_seed_counter++);

		auto d6 = std::uniform_int_distribution(1, 6);
		_mining_skill =
			d6(_default_random_engine) +
			d6(_default_random_engine) +
			d6(_default_random_engine);
	}
	Mob(
		TilePosition position,
		std::unique_ptr<MobBrain> brain
	) : MapObject(position), _brain(std::move(brain))
	{
		_default_random_engine =
			std::move(std::default_random_engine(_random_device()));
		_default_random_engine.seed(_rand_seed_counter++);

		auto d6 = std::uniform_int_distribution(1, 6);
		_mining_skill =
			d6(_default_random_engine) +
			d6(_default_random_engine) +
			d6(_default_random_engine);
	}
	Mob(
		TilePosition position,
		std::unique_ptr<MobBrain> brain,
		std::initializer_list<InternedString> tags
	) : MapObject(position, tags), _brain(std::move(brain))
	{
		_tags.insert(_tags.begin(), tags.begin(), tags.end());

		_default_random_engine =
			std::move(std::default_random_engine(_random_device()));
		_default_random_engine.seed(_rand_seed_counter++);

		auto d6 = std::uniform_int_distribution(1, 6);
		_mining_skill =
			d6(_default_random_engine) +
			d6(_default_random_engine) +
			d6(_default_random_engine);
	}
	virtual ~Mob() = default;

	double get_next_action_time() const noexcept { return _next_action_time; }
	double set_next_action_time(double new_value) noexcept
	{
		_next_action_time = new_value;
		return _next_action_time;
	}

	bool is_alive() const noexcept { return !is_dead(); }
	bool is_dead() const noexcept { return _is_dead; }

	void kill()
	{
		_is_dead = true;
		_brain->kill(*this);
	}

	std::default_random_engine& get_default_random_engine() noexcept
	{
		return _default_random_engine;
	}
	std::default_random_engine const& get_default_random_engine() const noexcept
	{
		return _default_random_engine;
	}

	std::random_device& get_random_device() noexcept
	{
		return _random_device;
	}
	std::random_device const& get_random_device() const noexcept
	{
		return _random_device;
	}

	void update(TileMap& map)
	{
		//_next_action_time -= 1.0;

		_brain->update(*this, map);

		if (map.size() != get_position())
		{
			kill();
			return;
		}

		if (map[get_position()] == TileGlyphIndex::Wall)
		{
			double mining_speed = (static_cast<double>(_mining_skill) / 10.0);
			_next_action_time += 5.0 / mining_speed;
		}
		else _next_action_time += 1.0;
	}

private:
	static unsigned int _rand_seed_counter;
	static std::random_device _random_device;

	std::unique_ptr<MobBrain> _brain;
	double _next_action_time = 1.0;
	std::default_random_engine _default_random_engine;
	uint16_t _mining_skill = 0;
	bool _is_dead = false;

	friend class PlayerBrain;
};

std::random_device Mob::_random_device;
unsigned int Mob::_rand_seed_counter = 0;