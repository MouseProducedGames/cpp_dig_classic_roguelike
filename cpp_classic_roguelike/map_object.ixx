#include "tags.hpp"

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module map_object;
#pragma warning( pop )

// local imports
import constants;
import glyph;
import tile_position;
import tile_displacement;

// std imports
import <utility>;
import <vector>;

export class MapObject
{
public:
	// de/constructors
	MapObject() {}
	MapObject(std::vector<TagName> tags)
	{
		_tags.insert(_tags.begin(), tags.begin(), tags.end());
	}
	MapObject(TilePosition position) : _position(position) {}
	MapObject(
		TilePosition position,
		std::vector<TagName> tags
	) : _position(position)
	{
		_tags.insert(_tags.begin(), tags.begin(), tags.end());
	}
	virtual ~MapObject() = default;

	// properties
	const TilePosition& get_position() const noexcept
	{
		return _position;
	}

	bool has_tag(const char* tag_char_string) const noexcept
	{
		return has_tag(TagName(tag_char_string));
	}
	bool has_tag(TagName tag) const noexcept
	{
		return std::find(_tags.begin(), _tags.end(), tag) != _tags.end();
	}

	const TilePosition& set_position(const TilePosition& new_pos) noexcept
	{
		_position = new_pos;
		return  _position;
	}

	// methods
	const TilePosition& move(const TileDisplacement& move_pos) noexcept
	{
		return set_position(get_position() + move_pos);
	}

protected:
	// Change to `<flat_set>` after Visual Studio implements it.
	std::vector<TagName> _tags;
	TilePosition _position;
};
