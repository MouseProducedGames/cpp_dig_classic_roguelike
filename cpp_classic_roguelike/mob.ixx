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

	void update()
	{
		_brain->update(*this);
	}

private:
	std::unique_ptr<MobBrain> _brain;
};
