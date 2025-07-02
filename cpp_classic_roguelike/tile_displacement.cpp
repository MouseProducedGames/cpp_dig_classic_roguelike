// this include
#include "tile_displacement.hpp"

TileDisplacement::TileDisplacement() : x(0), y(0) {}
TileDisplacement::TileDisplacement(uint8_t x, uint8_t y) : x(x), y(y) {}

TileDisplacement TileDisplacement::operator+(TileDisplacement rhs) const noexcept
{
	return TileDisplacement(x + rhs.x, y + rhs.y);
}
TileDisplacement TileDisplacement::operator-(TileDisplacement rhs) const noexcept
{
	return TileDisplacement(x - rhs.x, y - rhs.y);
}

bool TileDisplacement::operator==(TileDisplacement rhs) const noexcept
{
	return !(x != rhs.x || y != rhs.y);
}
bool TileDisplacement::operator!=(TileDisplacement rhs) const noexcept
{
	return !(*this == rhs);
}