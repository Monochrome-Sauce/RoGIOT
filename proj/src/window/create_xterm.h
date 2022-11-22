#ifndef WINDOW__CREATE_XTERM_H
#define WINDOW__CREATE_XTERM_H

#include <sys/types.h>
#include <stdio.h>


/* #Creates an input-output xterm terminal.
! To close the created terminal, call fclose() on the returned stream.
! This will not work unless the system has xterm in the PATH variables.
!
! @param childPid: pointer to an object which will store the child (xterm) process PID.
! @param title: C-string title for the created console. NULL will utilize a default title.
! @return: FILE* stream with both read and write permissions.
*/
FILE* create_xterm(pid_t *childPid, const char *title);


#endif /* WINDOW__CREATE_XTERM_H */
