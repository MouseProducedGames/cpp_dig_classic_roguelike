export module recti;

// local includes
//#include "tile_position.hpp"
import tile_position;

// std includes
import <stdint.h>;

export struct Recti
{
	Recti(int16_t left, int16_t right, int16_t top, int16_t bottom)
		: left(left), right(right), top(top), bottom(bottom)
	{
	}

	int16_t left;
	int16_t right;
	int16_t top;
	int16_t bottom;

	bool operator==(const TilePosition& position) const noexcept
	{
		return !(
			position.x < left || position.x > right ||
			position.y < top || position.y > bottom
			);
	}
	bool operator!=(const TilePosition& position) const noexcept
	{
		return !(*this == position);
	}
};
