#pragma once

// local includes
#include "tile_position.hpp"

// std includes
#include<cstdint>

struct TilePosition;

struct Recti
{
	Recti(int16_t left, int16_t right, int16_t top, int16_t bottom);

	int16_t left;
	int16_t right;
	int16_t top;
	int16_t bottom;

	bool operator==(const TilePosition& position) const noexcept;
	bool operator!=(const TilePosition& position) const noexcept;
};
