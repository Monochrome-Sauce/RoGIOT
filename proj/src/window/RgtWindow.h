#ifndef WINDOW__RGT_WINDOW_H
#define WINDOW__RGT_WINDOW_H
#include "rogiot/rgt.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>


struct RgtWindow
{
	FILE *terminal;
	pid_t child;
};


/* #Create a terminal window object.
! @param title: title of the created window.
! @param width: width of the created window. Must be a positive integer.
! @param height: height of the created window. Must be a positive integer.
! @return: a newly created object, or NULL on failure. Pass the object to RgtWindow::destroy()
when it's no longer used.
*/
RgtWindow* RgtWindow__create(const char *title, int width, int height);

/* #Destroy a terminal window object and clear its used resources.
! @param title: window object returned from RgtWindow::create().
*/
void RgtWindow__destroy(RgtWindow *window);

/* #Fetch the size of the XTERM window.
*/
struct winsize RgtWindow__get_size(const RgtWindow *window);

/* #Calculate the width in pixels using the number of columns.
*/
int RgtWindow__col_to_xpixels(int column);

/* #Calculate the height in pixels using the number of rows.
*/
int RgtWindow__row_to_ypixels(int row);

/* #Calculate the width in columns using the number of pixels.
*/
int RgtWindow__xpixels_to_col(int pixels);

/* #Calculate the height in rows using the number of pixels.
*/
int RgtWindow__ypixels_to_row(int pixels);


#endif /* WINDOW__RGT_WINDOW_H */
