// this include
#include "map_object.hpp"

// local includes
#include "constants.hpp"
#include "tile_displacement.hpp"

// std includes
#include<algorithm>

MapObject::MapObject(Glyph glyph) : _glyph(glyph) {}
MapObject::MapObject(Glyph glyph, TilePosition position) : _glyph(glyph), _position(position) {}

const Glyph& MapObject::get_glyph() const noexcept
{
	return _glyph;
}
const Glyph& MapObject::set_glyph(const Glyph& new_glyph) noexcept
{
	_glyph = new_glyph;
	return _glyph;
}

const TilePosition& MapObject::get_position() const noexcept
{
	return _position;
}
const TilePosition& MapObject::set_position(const TilePosition& new_pos) noexcept
{
	_position = new_pos;
	return  _position;
}

const TilePosition& MapObject::move(const TileDisplacement& move_pos) noexcept
{
	return set_position(get_position() + move_pos);
}