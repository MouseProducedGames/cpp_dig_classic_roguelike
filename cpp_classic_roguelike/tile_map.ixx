export module tile_map;

import base_map;

// std imports

export class TileMap : public BaseMap<char>
{
public:
	TileMap(std::uint8_t width, std::uint8_t height)
		: BaseMap(width, height, '#') {}

	void set_tile(TilePosition pos, char tile)
	{
		set_tile(pos.x, pos.y, tile);
	}
	void set_tile(std::uint8_t x, std::uint8_t y, char tile)
	{
		auto tile_index_maybe = _get_tile_index(x, y);
		if (tile_index_maybe.has_value())
		{
			_tiles[tile_index_maybe.value()] = tile;
		}			
	}
};