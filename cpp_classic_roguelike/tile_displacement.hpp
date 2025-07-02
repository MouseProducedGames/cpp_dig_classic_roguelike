#pragma once

// std includes
#include<stdint.h>

struct TileDisplacement
{
	TileDisplacement();
	TileDisplacement(uint8_t x, uint8_t y);

	int8_t x;
	int8_t y;

	TileDisplacement operator+(TileDisplacement rhs) const noexcept;
	TileDisplacement operator-(TileDisplacement rhs) const noexcept;

	bool operator==(TileDisplacement rhs) const noexcept;
	bool operator!=(TileDisplacement rhs) const noexcept;
};
