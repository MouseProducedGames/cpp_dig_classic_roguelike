#include "interned_string.hpp"
#include "level.hpp"
#include "skills.hpp"
#include "tags.hpp"

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module mob;
#pragma warning( pop )

// local imports
import action;
import glyph;
import map_object;
import match;
import tile_map;
import tile_position;

// std imports
import <chrono>;
import <limits>;
import <memory>;
import <random>;
import <unordered_map>;

typedef std::unordered_map<TagName, double> TagMultiplierLookup;
typedef std::unordered_map<SkillName, TagMultiplierLookup> SkillTagMultipllierLookup;

#define _3d6() (d6(_default_random_engine) + \
	d6(_default_random_engine) + \
	d6(_default_random_engine))

class Mob;

export class MobBrain
{
public:
	MobBrain() = default;
	virtual ~MobBrain() = default;

	virtual void kill(Mob& mob) {}

	virtual Action update(Mob& mob, Level& level) = 0;
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
		_skills[SKILL_MINING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_MINING, TAG_LARGE, 1.2);
		_skills[SKILL_FIGHTING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_FIGHTING, TAG_LARGE, 1.4);
	}
	Mob(
		std::unique_ptr<MobBrain> brain,
		std::vector<TagName> tags
	)
		: MapObject(tags), _brain(std::move(brain))
	{
		_default_random_engine =
			std::move(std::default_random_engine(_random_device()));
		_default_random_engine.seed(_rand_seed_counter++);

		auto d6 = std::uniform_int_distribution(1, 6);
		_skills[SKILL_MINING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_MINING, TAG_LARGE, 1.2);
		_skills[SKILL_FIGHTING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_FIGHTING, TAG_LARGE, 1.4);
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
		_skills[SKILL_MINING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_MINING, TAG_LARGE, 1.2);
		_skills[SKILL_FIGHTING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_FIGHTING, TAG_LARGE, 1.4);
	}
	Mob(
		TilePosition position,
		std::unique_ptr<MobBrain> brain,
		std::vector<TagName> tags
	) : MapObject(position, tags), _brain(std::move(brain))
	{
		_default_random_engine =
			std::move(std::default_random_engine(_random_device()));
		_default_random_engine.seed(_rand_seed_counter++);

		auto d6 = std::uniform_int_distribution(1, 6);
		_skills[SKILL_MINING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_MINING, TAG_LARGE, 1.2);
		_skills[SKILL_FIGHTING] = static_cast<uint16_t>(_3d6());
		set_skill_tag_multiplier(SKILL_FIGHTING, TAG_LARGE, 1.4);
	}
	virtual ~Mob() = default;

	double get_next_action_time() const noexcept
	{
		if (is_alive()) return _next_action_time;
		else return std::numeric_limits<double>::max();
	}
	double set_next_action_time(double new_value) noexcept
	{
		_next_action_time = new_value;
		return _next_action_time;
	}

	//uint16_t get_skill

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

	uint16_t get_raw_skill(SkillName skill_name)
	{
		return _skills.get_raw_skill(skill_name);
	}

	double get_skill_value(SkillName skill_name)
	{
		double value = _skills.get_skill_value(skill_name);
		//auto& tag_multiplier_lookup = get_tag_multiplier_lookup(skill_name);
		for (auto& tag : _tags)
		{
			value *= get_skill_tag_multiplier(skill_name, tag);
			/*auto it_tag_mul = tag_multiplier_lookup.find(tag);
			if (it_tag_mul == tag_multiplier_lookup.end()) continue;
			value *= it_tag_mul->second;*/
		}
		return value;
	}

	void pre_update(Level& level)
	{
		if (is_dead()) return;

		match(_next_action,
			[&](TileDisplacement move)
			{
				TilePosition next_pos = get_position() + move;
				if (level.get_tile_map().size() != next_pos)
				{
					kill();
					return;
				}


				bool collided = false;
				for (auto other_mob : level.iter_mobs_at(next_pos))
				{
					// should not match.
					if (this == &*other_mob || other_mob->is_dead()) continue;

					collided = true;

					double attack_skill = get_skill_value(SKILL_FIGHTING);
					double defence_skill = other_mob->get_skill_value(SKILL_FIGHTING);
					double chance = attack_skill / (attack_skill + defence_skill);
					double roll = std::uniform_real_distribution<double>()(
						_default_random_engine
						);

					if (roll <= chance)
					{
						other_mob->kill();
					}
				}

				if (!collided) set_position(next_pos);
			},
			[](std::nullopt_t) {}
		);

		/*_next_action_time = 0.0;
		_next_action = std::nullopt;*/
	}

	void update(Level& level)
	{
		if (is_dead()) return;

		_next_action = _brain->update(*this, level);
		_next_action_time = 1.0;

		match(_next_action,
			[&](TileDisplacement move)
			{
				TilePosition next_pos = get_position() + move;
				if (level.get_tile_map().size() != next_pos) return;

				if (level.get_tile_map()[next_pos] == TileGlyphIndex::Wall)
				{
					double mining_skill = get_skill_value(SKILL_MINING);
					double mining_speed = mining_skill / 10.0;
					_next_action_time = 5.0 / mining_speed;
				}
			},
			[&](std::nullopt_t) {}
		);
	}

private:
	static unsigned int _rand_seed_counter;
	static std::random_device _random_device;

	std::unique_ptr<MobBrain> _brain;
	double _next_action_time = 0.0;
	SkillTagMultipllierLookup _skill_tag_multipliers;
	Skills _skills;
	Action _next_action = std::nullopt;
	std::default_random_engine _default_random_engine;
	bool _is_dead = false;

	friend class PlayerBrain;

	TagMultiplierLookup& get_tag_multiplier_lookup(SkillName skill_name)
	{
		auto tag_mul_it = _skill_tag_multipliers.find(skill_name);
		if (tag_mul_it == _skill_tag_multipliers.end())
		{
			_skill_tag_multipliers.emplace(skill_name, TagMultiplierLookup());
		}

		return _skill_tag_multipliers[skill_name];
	}

	double get_skill_tag_multiplier(SkillName skill_name, TagName tag_name)
	{
		if (!skill_name || !tag_name) return 1.0;
		auto& lookup = get_tag_multiplier_lookup(skill_name);
		auto it = lookup.find(tag_name);
		if (it == lookup.end()) return 1.0;

		return it->second;
	}
	void set_skill_tag_multiplier(
		SkillName skill_name,
		TagName tag_name,
		double multiplyer
	)
	{
		if (!skill_name || !tag_name) return;

		get_tag_multiplier_lookup(skill_name)[tag_name] = multiplyer;
	}
};

std::random_device Mob::_random_device;
unsigned int Mob::_rand_seed_counter = 0;