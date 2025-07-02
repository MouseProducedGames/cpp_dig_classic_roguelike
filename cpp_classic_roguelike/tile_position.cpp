// this include
#include "tile_position.hpp"

// local includes
#include "tile_displacement.hpp"

TilePosition::TilePosition() : x(0), y(0) {}
TilePosition::TilePosition(uint8_t x, uint8_t y) : x(x), y(y) {}

TilePosition TilePosition::operator+(TileDisplacement rhs) const noexcept
{
	return TilePosition(x + rhs.x, y + rhs.y);
}
TilePosition TilePosition::operator-(TileDisplacement rhs) const noexcept
{
	return TilePosition(x - rhs.x, y - rhs.y);
}
TileDisplacement TilePosition::operator-(TilePosition rhs) const noexcept
{
	return TileDisplacement(
		(int8_t)((int16_t)x - (int16_t)rhs.x),
		(int8_t)((int16_t)y - (int16_t)rhs.y)
		);
}

bool TilePosition::operator==(TilePosition rhs) const noexcept
{
	return !(x != rhs.x || y != rhs.y);
}
bool TilePosition::operator!=(TilePosition rhs) const noexcept
{
	return !(*this == rhs);
}
