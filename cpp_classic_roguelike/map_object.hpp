#pragma once

// local includes
#include "glyph.hpp"
#include "tile_position.hpp"

class MapObject
{
public:
	// de/constructors
	MapObject(Glyph glyph);
	MapObject(Glyph glyph, TilePosition position);
	virtual ~MapObject() = default;

	// properties
	const Glyph& get_glyph() const noexcept;
	const Glyph& set_glyph(const Glyph& new_glyph) noexcept;

	const TilePosition& get_position() const noexcept;
	const TilePosition& set_position(const TilePosition& new_pos) noexcept;

	// methods
	const TilePosition& move(const TileDisplacement& move_pos) noexcept;

protected:
	TilePosition _position;
	Glyph _glyph;
};
