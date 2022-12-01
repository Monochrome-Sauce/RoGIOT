#ifndef WINDOW__XTERM__IO_H
#define WINDOW__XTERM__IO_H

#include "./struct.h"

#include <unistd.h>


ssize_t xterm__write(const struct XTerm *xterm, size_t len, const char buff[len]);

ssize_t xterm__read(const struct XTerm *xterm, size_t nbytes, char buff[nbytes]);


#endif /* WINDOW__XTERM__IO_H */
