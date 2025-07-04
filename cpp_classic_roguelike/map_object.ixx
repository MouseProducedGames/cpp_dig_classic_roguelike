export module map_object;

// local includes
#include "constants.hpp"

// local imports
//#include "tile_position.hpp"
import glyph;
import tile_position;
import tile_displacement;

export class MapObject
{
public:
	// de/constructors
	MapObject(Glyph glyph) : _glyph(glyph) {}
	MapObject(Glyph glyph, TilePosition position) : _glyph(glyph), _position(position) {}
	virtual ~MapObject() = default;

	// properties
	const Glyph& get_glyph() const noexcept
	{
		return _glyph;
	}
	const Glyph& set_glyph(const Glyph& new_glyph) noexcept
	{
		_glyph = new_glyph;
		return _glyph;
	}

	const TilePosition& get_position() const noexcept
	{
		return _position;
	}
	const TilePosition& set_position(const TilePosition& new_pos) noexcept
	{
		_position = new_pos;
		return  _position;
	}

	// methods
	const TilePosition& move(const TileDisplacement& move_pos) noexcept
	{
		return set_position(get_position() + move_pos);
	}

protected:
	TilePosition _position;
	Glyph _glyph;
};
