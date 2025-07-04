export module key_event;

// local imports
import virtual_key_code;
import virtual_scan_code;

export union KeyChar {
	char ascii_char;
	wchar_t unicode_char;
};

export struct KeyEvent {
	KeyChar key_char_union;
	VirtualKeyCode virtual_key_code;
	VirtualScanCode virtual_scan_code;
};
