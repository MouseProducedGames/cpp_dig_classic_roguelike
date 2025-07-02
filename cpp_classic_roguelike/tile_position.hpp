#pragma once

// std includes
#include<stdint.h>

struct TileDisplacement;

struct TilePosition
{
	TilePosition();
	TilePosition(uint8_t x, uint8_t y);

	uint8_t x;
	uint8_t y;

	TilePosition operator+(TileDisplacement rhs) const noexcept;
	TilePosition operator-(TileDisplacement rhs) const noexcept;
	TileDisplacement operator-(TilePosition rhs) const noexcept;

	bool operator==(TilePosition rhs) const noexcept;
	bool operator!=(TilePosition rhs) const noexcept;
};
