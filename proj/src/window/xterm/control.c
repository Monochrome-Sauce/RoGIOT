#include "../xterm.h"
#include "src/macros.h"

#include <limits.h>
#include <assert.h>
#include <unistd.h>


static bool inner__exec_command(const int fd, const int len, const char command[len])
{
	const ssize_t res = write(fd, command, (size_t)len);
	assert(res == len);
	return res == len;
}

#define XTERM_ESCAPE_COMMAND(fd, commandString) inner__exec_command(fd, SIZEOF_ARRAY(commandString), "\e" commandString)
#define XTERM_ESCSEQ_MAX 64



bool xterm__clear_screen(const int fd)
{
	return XTERM_ESCAPE_COMMAND(fd, "c");
}

bool xterm__hide_cursor(const int fd)
{
	return XTERM_ESCAPE_COMMAND(fd, "[?25l");
}

bool xterm__move_cursor_to(const int fd, int row, int col)
{
	char buff[XTERM_ESCSEQ_MAX] = { 0 };
	const int len = snprintf(buff, SIZEOF_ARRAY(buff), "\e[%d;%dH", row, col);
	return inner__exec_command(fd, len, buff);
}

