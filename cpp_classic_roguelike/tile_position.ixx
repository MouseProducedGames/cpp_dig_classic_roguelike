export module tile_position;

// local imports
import tile_displacement;

// std imports
import <cstdint>;

export struct TilePosition
{
	TilePosition() : x(0), y(0) {}
	TilePosition(uint8_t x, uint8_t y) : x(x), y(y) {}

	std::uint8_t x;
	std::uint8_t y;

	TilePosition operator+(TileDisplacement rhs) const noexcept
	{
		return TilePosition(x + rhs.x, y + rhs.y);
	}
	TilePosition operator-(TileDisplacement rhs) const noexcept
	{
		return TilePosition(x - rhs.x, y - rhs.y);
	}
	TileDisplacement operator-(TilePosition rhs) const noexcept
	{
		return TileDisplacement(
			(int8_t)((int16_t)x - (int16_t)rhs.x),
			(int8_t)((int16_t)y - (int16_t)rhs.y)
		);
	}

	bool operator==(TilePosition rhs) const noexcept
	{
		return !(x != rhs.x || y != rhs.y);
	}

	bool operator!=(TilePosition rhs) const noexcept
	{
		return !(*this == rhs);
	}
};
