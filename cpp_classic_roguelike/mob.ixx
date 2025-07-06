export module player;


// local imports
import glyph;
import map_object;
import tile_position;

export class Mob : public MapObject
{
public:
	// de/constructors
	Mob(Glyph glyph) : MapObject(glyph) {}
	Mob(Glyph glyph, TilePosition position) : MapObject(glyph, position) {}
	virtual ~Mob() = default;
};
