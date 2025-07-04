// local includes
#include "../../console.hpp"

// std includes
#include<array>
#include<vector>

class WinConsole : public Console
{
public:
	WinConsole(char width, char height);
	virtual ~WinConsole() = default;

	/*virtual Sizei get_console_size() const;*/

	virtual std::optional<KeyEvent> read_key();

	virtual void present();

	virtual void set_console_height(char height);
	virtual void set_console_width(char width);

	virtual void set_full_screen(bool on);

	virtual void write(char ch);
	virtual void write(char ch, char x, char y);

protected:
	std::array<std::vector<char>, 2> _buffers;
	std::size_t _back_buffer_index;
	std::size_t _fore_buffer_index() const;

	std::size_t _cursor_tile_index();
};