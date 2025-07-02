#pragma once
// this includes
#include "mob.hpp"

class Player : public Mob
{
public:
	// de/constructors
	Player(Glyph glyph);
	Player(Glyph glyph, TilePosition position);
	virtual ~Player() = default;
};