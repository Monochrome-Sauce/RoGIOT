#include "../xterm.h"

#include <unistd.h>


ssize_t xterm__write(const struct XTerm *xterm, const size_t len, const char buff[len])
{
	return write(xterm->fd, buff, len);
}

ssize_t xterm__read(const struct XTerm *xterm, const size_t nbytes, char buff[nbytes])
{
	return read(xterm->fd, buff, nbytes);
}
