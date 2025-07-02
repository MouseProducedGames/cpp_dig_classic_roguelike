// this include
#include "mob.hpp"

// local includes
#include "constants.hpp"
#include "tile_displacement.hpp"

// std includes
#include<algorithm>

Mob::Mob(Glyph glyph) : _glyph(glyph) {}
Mob::Mob(Glyph glyph, TilePosition position) : _glyph(glyph), _position(position) {}

const Glyph& Mob::get_glyph() const noexcept
{
	return _glyph;
}
const Glyph& Mob::set_glyph(const Glyph& new_glyph) noexcept
{
	_glyph = new_glyph;
	return _glyph;
}

const TilePosition& Mob::get_position() const noexcept
{
	return _position;
}
const TilePosition& Mob::set_position(const TilePosition& new_pos) noexcept
{
	_position = new_pos;
	return  _position;
}

const TilePosition& Mob::move(const TileDisplacement& move_pos) noexcept
{
	return set_position(get_position() + move_pos);
}