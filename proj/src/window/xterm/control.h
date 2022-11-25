#ifndef WINDOW__XTERM__CONTROL_H
#define WINDOW__XTERM__CONTROL_H

#include <stdbool.h>
#include <stdio.h>


/*
! Note: the cursor returns to its default visibility state (shown), it's adviced to
follow this call with a call to xterm::hide_cursor().
*/
bool xterm__clear_screen(FILE *s);

bool xterm__hide_cursor(FILE *s);

bool xterm__move_cursor_to(FILE *s, int row, int col);


#endif /* WINDOW__XTERM__CONTROL_H */
