export module tile_displacement;

// std imports
import <cstdint>;

export struct TileDisplacement
{
	TileDisplacement() : x(0), y(0) {}
	TileDisplacement(uint8_t x, uint8_t y) : x(x), y(y) {}

	int8_t x;
	int8_t y;

	TileDisplacement operator+(TileDisplacement rhs) const noexcept
	{
		return TileDisplacement(x + rhs.x, y + rhs.y);
	}
	TileDisplacement operator-(TileDisplacement rhs) const noexcept
	{
		return TileDisplacement(x - rhs.x, y - rhs.y);
	}

	bool operator==(TileDisplacement rhs) const noexcept
	{
		return !(x != rhs.x || y != rhs.y);
	}
	bool operator!=(TileDisplacement rhs) const noexcept
	{
		return !(*this == rhs);
	}
};
