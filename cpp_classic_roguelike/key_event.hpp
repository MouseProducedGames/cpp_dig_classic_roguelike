#pragma once

// local includes
#include "virtual_key_code.hpp"
#include "virtual_scan_code.hpp"

typedef union u_KeyChar {
	char ascii_char;
	wchar_t unicode_char;
} KeyChar;

typedef struct s_KeyEvent {
	KeyChar key_char_union;
	VirtualKeyCode virtual_key_code;
	VirtualScanCode virtual_scan_code;
} KeyEvent;
