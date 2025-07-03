#pragma once

// local includes
#include "tile_position.hpp"

// std includes
#include<cstdint>

struct Sizei
{
	Sizei(uint8_t width, uint8_t height);

	uint8_t width;
	uint8_t height;

	bool operator==(const TilePosition& position) const noexcept;
	bool operator!=(const TilePosition& position) const noexcept;
};
