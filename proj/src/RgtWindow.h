#ifndef RGT_WINDOW_H
#define RGT_WINDOW_H
#include "rogiot/rgt.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>


struct RgtWindow
{
	FILE *terminal;
	pid_t child;
};


/* #Create a terminal window object.
! @param title: title of the created window.
! @return: a newly created object, or NULL on failure. Pass the object to RgtWindow::destroy()
when it's no longer used.
*/
RgtWindow* RgtWindow__create(const char *title);

/* #Destroy a terminal window object and clear its used resources.
! @param title: window object returned from RgtWindow::create().
*/
void RgtWindow__destroy(RgtWindow *window);


#endif /* RGT_WINDOW_H */
