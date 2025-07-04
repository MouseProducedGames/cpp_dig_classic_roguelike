export module base_map;

// local imports
import sizei;
import tile_position;

// std imports
import <optional>;
import <vector>;

export template<typename T> class BaseMap {
public:
	// constructors
	BaseMap(std::uint8_t width, std::uint8_t height, T value) : _width(width), _height(height)
	{
		//_tiles.reserve(width * height);
		_tiles.resize(width * height, value);
	}
	BaseMap(BaseMap& copy) = delete; // One soource for truth.
	BaseMap(BaseMap&& move) = delete; // One soource for truth.
	virtual ~BaseMap() = default;

	// properties
	std::uint8_t height() const noexcept { return _height; }
	Sizei size() const noexcept { return Sizei(width(), height()); }
	std::uint8_t width() const noexcept { return _width; }

	// methods
	std::optional<T> get_tile(TilePosition pos) const noexcept
	{
		return get_tile(pos.x, pos.y);
	}
	std::optional<T> get_tile(std::uint8_t x, std::uint8_t y) const noexcept     
	{
		auto tile_index_maybe = _get_tile_index(x, y);
		if (tile_index_maybe.has_value())
		{
			auto tile_index = tile_index_maybe.value();
			return _tiles[tile_index];
		}
		else
			return {};
	}

	bool is_on_map(TilePosition pos) const noexcept {
		return is_on_map(pos.x, pos.y);
	}
	bool is_on_map(std::uint8_t x, std::uint8_t y) const noexcept {
		return !(x >= _width || y >= _height);
	}

	// operators
	T const& operator[](TilePosition pos) const {
		auto tile_index_maybe = _get_tile_index(pos.x, pos.y);
		auto tile_index = tile_index_maybe.value();
		return _tiles[tile_index];
	}
	T const& operator[](std::uint8_t x, std::uint8_t y) const
	{
		auto tile_index_maybe = _get_tile_index(x, y);
		auto tile_index = tile_index_maybe.value();
		return _tiles[tile_index];
	}

protected:
	std::vector<T> _tiles;

	std::optional<std::size_t> _tile_index(TilePosition pos)
	{
		_tile_index(pos.x, pos.y);
	}
	std::optional<std::size_t> _tile_index(
		std::uint8_t x,
		std::uint8_t y
	) const noexcept
	{
		static_assert(x < width());
		static_assert(y < height());
		return (y * width()) + x;
	}

	std::optional<std::size_t> _get_tile_index(TilePosition pos)
	{
		return _get_tile_index(pos.x, pos.y);
	}
	std::optional<std::size_t> _get_tile_index(
		std::uint8_t x,
		std::uint8_t y
	) const noexcept
	{
		if (is_on_map(x, y))
			return (y * width()) + x;
		else
			return {};
	}

private:
	std::uint8_t _height;
	std::uint8_t _width;
};
