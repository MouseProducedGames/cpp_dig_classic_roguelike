export module player;

// local imports
import console;
import mob;
import tile_displacement;
import virtual_scan_code;

export class PlayerBrain : public MobBrain
{
	void update(Mob& mob)
	{
		auto key_maybe = Console::instance().read_key();
		if (key_maybe.has_value())
		{
			//std::print("{}", (unsigned int)key_maybe.value().virtual_scan_code);
			TileDisplacement move;
			switch (key_maybe.value().virtual_scan_code)
			{
			case VirtualScanCode::Q:
			case VirtualScanCode::Numpad7: move.x = -1; move.y = -1; break;
			case VirtualScanCode::W:
			case VirtualScanCode::Numpad8: move.y = -1; break;
			case VirtualScanCode::E:
			case VirtualScanCode::Numpad9: move.x = 1; move.y = -1; break;
			case VirtualScanCode::A:
			case VirtualScanCode::Numpad4: move.x = -1; break;
			case VirtualScanCode::S:
			case VirtualScanCode::Numpad5: break;
			case VirtualScanCode::D:
			case VirtualScanCode::Numpad6: move.x = 1; break;
			case VirtualScanCode::Z:
			case VirtualScanCode::Numpad1: move.x = -1; move.y = 1; break;
			case VirtualScanCode::X:
			case VirtualScanCode::Numpad2: move.y = 1; break;
			case VirtualScanCode::C:
			case VirtualScanCode::Numpad3: move.x = 1; move.y = 1; break;
			}

			mob.move(move);
		}
	}
};