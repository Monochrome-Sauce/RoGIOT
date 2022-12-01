#ifndef WINDOW__XTERM__CREATE_H
#define WINDOW__XTERM__CREATE_H

#include "./struct.h"

#include <sys/types.h>
#include <stdbool.h>


#define XTERM_FONT_W 5
#define XTERM_FONT_H 7


/* #Creates an input-output xterm terminal.
! This function will not work unless the system has xterm in the PATH variables.
! To close the created terminal, call xterm::close() on the returned object.
!
! @param childPid: pointer to an object which will store the child (xterm) process PID.
! @param title: C-string title for the created console. NULL will utilize a default title.
! @param columns: number of character columns; each column is XTERM_FONT_W pixels.
! @param rows: number of character rows; each row is XTERM_FONT_H pixels.
! @return: XTerm structure.
!
! childPid must be a valid pointer.
! width and height must be a positive integers.
*/
struct XTerm xterm__create(pid_t *childPid, const char title[], int columns, int rows);

/* #Close a valid XTerm object returned from xterm::create().
! The object will be marked as invalid, causing xterm::isvalid() to return false.
*/
void xterm__close(struct XTerm *x);

bool xterm__isvalid(const struct XTerm *x);


#endif /* WINDOW__XTERM__CREATE_H */
