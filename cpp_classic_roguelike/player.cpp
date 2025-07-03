// this include
#include "player.hpp"

Player::Player(Glyph glyph) : MapObject(glyph) {}
Player::Player(Glyph glyph, TilePosition position) : MapObject(glyph, position) {}
