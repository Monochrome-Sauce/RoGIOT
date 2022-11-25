#include "../xterm.h"
#include "src/macros.h"

#include <limits.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>


static bool inner__exec_command(FILE *const s, const size_t len, const char command[len])
{
	const size_t res = fwrite(command, 1, len, s);
	
	assert(res == len);
	return res == len;
}

__attribute__((format(printf, 2, 3)))
static bool inner__exec_formatted(FILE *const s, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	const int res = vfprintf(s, fmt, args);
	va_end(args);
	
	assert(res > 0);
	return res > 0;
}

#define XTERM_ESCAPE_COMMAND(fstream, commandString) inner__exec_command(fstream, SIZEOF_ARRAY(commandString), "\e" commandString)



bool xterm__clear_screen(FILE *const s)
{
	return XTERM_ESCAPE_COMMAND(s, "c");
}

bool xterm__hide_cursor(FILE *const s)
{
	return XTERM_ESCAPE_COMMAND(s, "[?25l");
}

bool xterm__move_cursor_to(FILE *const s, const int row, const int col)
{
	return inner__exec_formatted(s, "\e[%d;%dH", row, col);
}

