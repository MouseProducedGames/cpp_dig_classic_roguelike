#include "level.hpp"

// Include files (currently) must go above the module export.
#pragma warning( push )
#pragma warning( disable : 5201 )
export module player;
#pragma warning( pop )

// local imports
import action;
import console;
import mob;
import tile_displacement;
import virtual_scan_code;

export class PlayerBrain : public MobBrain
{
	Action update(Mob& mob, Level& level)
	{
		//mob._mining_skill = 3;

		auto key = Console::instance().wait_key();
		typedef TileDisplacement TD;
		switch (key.virtual_scan_code)
		{
		case VirtualScanCode::Z:
		case VirtualScanCode::Numpad1: return TD(-1, 1);
		case VirtualScanCode::X:
		case VirtualScanCode::Numpad2: return TD(0, 1);
		case VirtualScanCode::C:
		case VirtualScanCode::Numpad3: return TD(1, 1);
		case VirtualScanCode::A:
		case VirtualScanCode::Numpad4: return TD(-1, 0);
		case VirtualScanCode::S:
		case VirtualScanCode::Numpad5: return std::nullopt;
		case VirtualScanCode::D:
		case VirtualScanCode::Numpad6: return TD(1, 0);
		case VirtualScanCode::Q:
		case VirtualScanCode::Numpad7: return TD(-1, -1);
		case VirtualScanCode::W:
		case VirtualScanCode::Numpad8: return TD(0, -1);
		case VirtualScanCode::E:
		case VirtualScanCode::Numpad9: return TD(1, -1);
		default: return std::nullopt;
		}
	}
};