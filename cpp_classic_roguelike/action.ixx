export module action;

// local imports
import tile_displacement;

// std imports
import <optional>;
import <variant>;

export typedef std::variant<TileDisplacement, std::nullopt_t> Action;