// local includes
#include "../../console.hpp"

// std includes
#include<array>
#include<vector>

class WinConsole : public Console
{
public:
	WinConsole();
	virtual ~WinConsole() = default;

	virtual std::optional<KeyEvent> read_key();

	virtual void present();

	virtual void write(char ch);
	virtual void write(char ch, char x, char y);

protected:
	std::array<std::vector<char>, 2> _buffers;
	std::size_t _back_buffer_index;
	std::size_t _fore_buffer_index();

	std::size_t _cursor_tile_index();
};