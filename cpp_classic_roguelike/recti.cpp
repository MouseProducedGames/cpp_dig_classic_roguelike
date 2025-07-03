// this include
#include "recti.hpp"

Recti::Recti(int16_t left, int16_t right, int16_t top, int16_t bottom)
	: left(left), right(right), top(top), bottom(bottom)
{}

bool Recti::operator==(const TilePosition& position) const noexcept
{
	return !(
		position.x < left || position.x > right ||
		position.y < top || position.y > bottom
		);
}
bool Recti::operator!=(const TilePosition& position) const noexcept
{
	return !(*this == position);
}
