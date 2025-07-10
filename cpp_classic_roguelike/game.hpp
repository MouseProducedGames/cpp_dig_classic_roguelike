// local imports
import mob;
import tile_map;

// std imports
import <memory>;
import <random>;
import <vector>;

void advance_time(std::vector<std::shared_ptr<Mob>>& mobs);

void clean_mobs(std::vector<std::shared_ptr<Mob>>& mobs);
void initial_mobs_spawn(
	std::vector<std::shared_ptr<Mob>>& mobs,
	std::default_random_engine& random_device_impl,
	std::uniform_int_distribution<int>& rand_x_pos,
	std::uniform_int_distribution<int>& rand_y_pos
);
std::shared_ptr<Mob> initial_player_spawn(std::vector<std::shared_ptr<Mob>>& mobs);
void update_mobs(std::vector<std::shared_ptr<Mob>>& mobs, TileMap& map);
