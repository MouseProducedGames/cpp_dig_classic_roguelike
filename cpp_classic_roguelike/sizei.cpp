// this include
#include "sizei.hpp"

Sizei::Sizei(uint8_t width, uint8_t height)
	: width(width), height(height)
{
}

bool Sizei::operator==(const TilePosition& position) const noexcept
{
	return !(position.x > width || position.y > height);
}
bool Sizei::operator!=(const TilePosition& position) const noexcept
{
	return !(*this == position);
}