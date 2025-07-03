#pragma once
// this includes
#include "map_object.hpp"

class Player : public MapObject
{
public:
	// de/constructors
	Player(Glyph glyph);
	Player(Glyph glyph, TilePosition position);
	virtual ~Player() = default;
};