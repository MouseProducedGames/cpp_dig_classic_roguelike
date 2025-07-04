export module sizei;

// local includes
//#include "tile_position.hpp"
import tile_position;

// std includes
import <stdint.h>;

export struct Sizei
{
	Sizei(uint8_t width, uint8_t height)
		: width(width), height(height)
	{
	}

	uint8_t width;
	uint8_t height;

	bool operator==(const TilePosition& position) const noexcept
	{
		return !(position.x > width || position.y > height);
	}
	bool operator!=(const TilePosition& position) const noexcept
	{
		return !(*this == position);
	}
};
