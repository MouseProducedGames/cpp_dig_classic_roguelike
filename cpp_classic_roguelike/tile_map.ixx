export module tile_map;

import base_map;

// std imports

export class TileMap : public BaseMap<char>
{
public:
	TileMap(std::size_t x, std::size_t y) : BaseMap(x, y, '#') {}

	void set_tile(std::size_t x, std::size_t y, char tile)
	{
		auto tile_index_maybe = get_tile_index(x, y);
		if (tile_index_maybe.has_value())
		{
			_tiles[tile_index_maybe.value()] = tile;
		}			
	}
};