#define _POSIX_C_SOURCE 1
#include "./RgtWindow.h"
#include "src/window/create_xterm.h"
#include "src/macros.h"

#include <math.h>
#include <stdlib.h>


RgtWindow* RgtWindow__create(const char *const title, int width, int height)
{
	assert(width > 0 && height > 0);
	
	if unlikely (width < XTERM_FONT_W) {
		width = XTERM_FONT_W;
	}
	if unlikely (height < XTERM_FONT_H) {
		height = XTERM_FONT_H;
	}
	
	RgtWindow *window = rgt_new(RgtWindow);
	if likely (window != NULL) {
		const int cols = RgtWindow__xpixels_to_col(width);
		const int rows = RgtWindow__ypixels_to_row(height);
		window->terminal = create_xterm(&window->child, title, cols, rows);
		
		if (window->terminal == NULL) {
			rgt_free(window);
		}
	}
	return window;
}

void RgtWindow__destroy(RgtWindow *window)
{
	assert(window != NULL);
	
	fclose(window->terminal);
	rgt_free(window);
}

struct winsize RgtWindow__get_size(const RgtWindow *const window)
{
	assert(window != NULL);
	
	struct winsize ws = { 0 };
	ioctl(fileno(window->terminal), TIOCGWINSZ, &ws);
	return ws;
}

int RgtWindow__col_to_xpixels(const int column)
{
	return column * XTERM_FONT_W;
}

int RgtWindow__row_to_ypixels(const int row)
{
	return row * XTERM_FONT_H;;
}

int RgtWindow__xpixels_to_col(const int pixels)
{
	return (int)floor(pixels / (double)XTERM_FONT_W);
}

int RgtWindow__ypixels_to_row(const int pixels)
{
	return (int)floor(pixels / (double)XTERM_FONT_H);
}
