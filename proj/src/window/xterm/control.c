#include "../xterm.h"
#include "src/macros.h"

#include <limits.h>
#include <unistd.h>


static bool inner__exec_command(const int fd, const int len, const char command[len])
{
	const ssize_t res = write(fd, command, (size_t)len);
	return res == len;
}

#define XTERM_ESCAPE_COMMAND(fd, commandString) inner__exec_command(fd, SIZEOF_ARRAY(commandString), "\e" commandString)



bool xterm__hide_cursor(const int fd)
{
	return XTERM_ESCAPE_COMMAND(fd, "[?25l");
}

bool xterm__clear_screen(const int fd)
{
	return XTERM_ESCAPE_COMMAND(fd, "c");
}

bool xterm__move_cursor_to(const int fd, int row, int col)
{
	char buff[sizeof ("\e;") + sizeof (TOKSTR(INT_MAX)) * 2];
	const int len = snprintf(buff, sizeof (buff), "\e%d;%d", row, col);
	return inner__exec_command(fd, len, buff);
}

