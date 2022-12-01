#include "../xterm.h"
#include "src/macros.h"

#include <limits.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>


#define CONTROL_FORMAT_BUFF_SIZE (2 * sizeof (int) * 8)

static bool inner__exec_command(const struct XTerm *x, const unsigned int len, const char command[len])
{
	const ssize_t res = xterm__write(x, len, command);
	
	assert(res == len);
	return res == len;
}

#define XTERM_ESCAPE_COMMAND(xtermStruct, commandString) \
	inner__exec_command(xtermStruct, SIZEOF_ARRAY(commandString), "\e" commandString)


bool xterm__clear_screen(const struct XTerm *x)
{
	return XTERM_ESCAPE_COMMAND(x, "c");
}

bool xterm__hide_cursor(const struct XTerm *x)
{
	return XTERM_ESCAPE_COMMAND(x, "[?25l");
}

bool xterm__move_cursor_to(const struct XTerm *x, const int row, const int col)
{
	char buff[CONTROL_FORMAT_BUFF_SIZE] = { 0 };
	unsigned int len = (unsigned)snprintf(buff, SIZEOF_ARRAY(buff), "\e[%d;%dH", row, col);
	
	return inner__exec_command(x, len, buff);
}
