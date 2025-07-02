#pragma once

#include<vector>
#include<optional>

template<typename T> class BaseMap {
public:
	// constructors
	BaseMap(std::size_t width, std::size_t height, T value) : _width(width), _height(height)
	{
		//_tiles.reserve(width * height);
		_tiles.resize(width * height, value);
	}
	BaseMap(BaseMap& copy) = delete; // One soource for truth.
	BaseMap(BaseMap&& move) = delete; // One soource for truth.
	virtual ~BaseMap() = default;

	// properties
	std::size_t height() const noexcept { return _height; }
	std::size_t width() const noexcept { return _width; }

	// methods
	std::optional<T> get_tile(std::size_t x, std::size_t y) const noexcept
	{
		auto tile_index_maybe = get_tile_index(x, y);
		if (tile_index_maybe.has_value())
		{
			std::size_t tile_index = tile_index_maybe.value();
			return _tiles[tile_index];
		}
		else
			return std::nullopt;
	}

	// Visual Studio C++ *does not* support multidimensional subscript operators.
	/*T const& operator[](std::size_t x, std::size_t y) const
	{
		auto tile_index_maybe = get_tile_index(x, y);
		std::size_t tile_index = tile_index_maybe.value();
		return _tiles[tile_index];
	}*/

	bool is_on_map(std::size_t x, std::size_t y) const noexcept {
		return !(x >= _width || y >= _height);
	}

protected:
	std::vector<T> _tiles;

	std::optional<std::size_t> get_tile_index(std::size_t x, std::size_t y) const noexcept
	{
		if (is_on_map(x, y))
			return (y * width()) + x;
		else
			return std::nullopt;
	}

private:
	std::size_t _height;
	std::size_t _width;
};
