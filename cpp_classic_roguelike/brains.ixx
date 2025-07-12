#include "level.hpp"

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module brains;
#pragma warning( pop )

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
		_choice = std::uniform_int_distribution<int>(1, 9);
	}
	~RandomMoveBrain() = default;

	void update(Mob& mob, Level& level)
	{
		int choice_roll = _choice(mob.get_default_random_engine());
		TileDisplacement move;
		switch (choice_roll)
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
	std::uniform_int_distribution<int> _choice;
};