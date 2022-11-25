#ifndef WINDOW__XTERM__CONTROL_H
#define WINDOW__XTERM__CONTROL_H

#include <stdbool.h>


bool xterm__hide_cursor(int fd);
bool xterm__clear_screen(int fd);
bool xterm__move_cursor_to(int fd, int row, int col);


#endif /* WINDOW__XTERM__CONTROL_H */
