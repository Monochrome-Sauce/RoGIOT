#ifndef ROGIOT__RGT_H
#define ROGIOT__RGT_H

#define _POSIX_C_SOURCE 1
#include <stdbool.h>
#include <stdio.h>


/* #Initiates the library, stealing the standard streams (stdin, stdout, stderr).
! The standard streams are still usable, but for the library to work properly
please refrain from utilizing them and refer to rgt::create_debug_output() for
creating a debug-output console.
!
! Must be called only once.
*/
bool rgt__init(void);

/* #Clears the resources used by the library.
!
! Must be called after rgt::init().
*/
void rgt__deinit(void);


/* #Creates an xterm terminal with write permissions only.
! To close the created terminal, just call fclose() on the returned stream.
! This will not work unless your system has xterm in the PATH variables.
!
! @param title: C-string title for the created console. Pass NULL for a default title.
! @return: Line-buffered write-only FILE* stream.
*/
FILE* rgt__create_debug_output(const char *title);


#endif /* ROGIOT__RGT_H */
