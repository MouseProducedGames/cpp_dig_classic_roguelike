export module mob;

// local imports
import glyph;
import map_object;
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

	virtual void update(Mob& mob) = 0;
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

	bool is_alive() const noexcept { return !is_dead(); }
	bool is_dead() const noexcept { return _is_dead; }

	void kill()
	{
		_is_dead = true;
		_brain->kill(*this);
	}

	void update()
	{
		_brain->update(*this);
	}

private:
	std::unique_ptr<MobBrain> _brain;
	bool _is_dead = false;
};
