#ifndef WINDOW__XTERM__CREATE_H
#define WINDOW__XTERM__CREATE_H

#include <sys/types.h>
#include <stdio.h>


#define XTERM_FONT_W 5
#define XTERM_FONT_H 7


/* #Creates an input-output xterm terminal.
! To close the created terminal, call fclose() on the returned stream.
! This will not work unless the system has xterm in the PATH variables.
!
! @param childPid: pointer to an object which will store the child (xterm) process PID.
! @param title: C-string title for the created console. NULL will utilize a default title.
! @param columns: number of character columns; each column is XTERM_FONT_W pixels.
! @param rows: number of character rows; each row is XTERM_FONT_H pixels.
! @return: FILE* stream with both read and write permissions.
!
! childPid must be a valid pointer.
! width and height must be a positive integers.
*/
FILE* xterm__create(pid_t *childPid, const char title[], int columns, int rows);


#endif /* WINDOW__XTERM__CREATE_H */
