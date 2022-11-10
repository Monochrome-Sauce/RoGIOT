#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "Pipe.h"


/* #the child's threads and library's main.
! @param comms: Bi-directional pipe to communicate with the calling thread.
! @param term: Bi-directional pipe to communicate with the terminal.
*/
int mainloop(const struct Pipe comms, const struct Pipe term);


#endif /* MAINLOOP_H */
