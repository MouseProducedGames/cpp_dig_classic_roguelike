export module brains;

// local imports
import mob;
import tile_displacement;

// std imports
import <random>;

export class RandomMoveBrain : public MobBrain
{
public:
	RandomMoveBrain()
	{
		e1 = std::move(std::default_random_engine(r()));
	}
	~RandomMoveBrain() = default;

	void update(Mob& mob, TileMap& map)
	{
		int choice = std::uniform_int_distribution<int>(1, 9)(e1);
		TileDisplacement move;
		switch (choice)
		{
		case 1: move.x = -1; move.y = -1; break;
		case 2: move.y = -1; break;
		case 3: move.x = 1; move.y = -1; break;
		case 4: move.x = -1; break;
		case 5: break;
		case 6: move.x = 1; break;
		case 7: move.x = -1; move.y = 1; break;
		case 8: move.y = 1; break;
		case 9: move.x = 1; move.y = 1; break;
		}

		mob.move(move);
	}

private:
	std::random_device r;
	std::default_random_engine e1;
};