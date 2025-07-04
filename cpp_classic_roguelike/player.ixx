export module player;


// local imports
import glyph;
import map_object;
import tile_position;

export class Player : public MapObject
{
public:
	// de/constructors
	Player(Glyph glyph) : MapObject(glyph) {}
	Player(Glyph glyph, TilePosition position) : MapObject(glyph, position) {}
	virtual ~Player() = default;
};
