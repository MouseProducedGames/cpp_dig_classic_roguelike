#include "level.hpp"

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module brains;
#pragma warning( pop )

// local imports
import action;
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

	Action update(Mob& mob, Level& level)
	{
		int choice_roll = _choice(mob.get_default_random_engine());
		typedef TileDisplacement TD;
		switch (choice_roll)
		{
		case 1: return TD(-1, -1);
		case 2: return TD(0, -1);
		case 3: return TD(1, -1);
		case 4: return TD(-1, 0);
		case 5: return std::nullopt;
		case 6: return TD(1, 0);
		case 7: return TD(-1, +1);
		case 8: return TD(0, +1);
		case 9: return TD(1, +1);
		default: return std::nullopt;
		}
	}

private:
	std::uniform_int_distribution<int> _choice;
};