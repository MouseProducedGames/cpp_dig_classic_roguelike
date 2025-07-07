export module mob;

// local imports
import glyph;
import map_object;
import tile_map;
import tile_position;

// std imports
import <memory>;

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
	Mob(Glyph glyph, std::unique_ptr<MobBrain> brain)
		: MapObject(glyph), _brain(std::move(brain)) {}
	Mob(
		Glyph glyph,
		TilePosition position,
		std::unique_ptr<MobBrain> brain
	) : MapObject(glyph, position), _brain(std::move(brain)) {}
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
			_next_action_time += 5.0;
		else _next_action_time += 1.0;
	}

private:
	std::unique_ptr<MobBrain> _brain;
	bool _is_dead = false;
	double _next_action_time = 1.0;
};
