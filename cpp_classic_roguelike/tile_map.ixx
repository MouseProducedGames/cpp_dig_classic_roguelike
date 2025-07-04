export module tile_map;

// this imports
import base_map;

// local imports
import glyph;

export class TileMap : public BaseMap<TileGlyphIndex>
{
public:
	TileMap(std::uint8_t width, std::uint8_t height)
		: BaseMap(width, height, TileGlyphIndex::Wall) { }

	void set_tile(TilePosition pos, TileGlyphIndex tile)
	{
		set_tile(pos.x, pos.y, tile);
	}
	void set_tile(std::uint8_t x, std::uint8_t y, TileGlyphIndex tile)
	{
		auto tile_index_maybe = _get_tile_index(x, y);
		if (tile_index_maybe.has_value())
		{
			_tiles[tile_index_maybe.value()] = tile;
		}			
	}
};