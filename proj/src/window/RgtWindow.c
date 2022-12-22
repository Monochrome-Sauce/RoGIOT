#include "./RgtWindow.h"
#include "src/window/xterm.h"
#include "src/macros.h"

#include <math.h>
#include <stdlib.h>


static void inner__cleanup_failed_create(RgtWindow *wnd)
{
	if (xterm__isvalid(&wnd->terminal)) {
		close(wnd->terminal.fd);
	}
	rgt_free(wnd);
}

RgtWindow* RgtWindow__create(const char *const title, int width, int height)
{
	assert(0 < width && width < UINT16_MAX);
	assert(0 < height && height < UINT16_MAX);
	
	if unlikely (width < XTERM_FONT_W) {
		width = XTERM_FONT_W;
	}
	if unlikely (height < XTERM_FONT_H) {
		height = XTERM_FONT_H;
	}
	
	
	RgtWindow *window = rgt_new(RgtWindow);
	if unlikely (window == NULL) {
		return NULL;
	}
	
	const int cols = RgtWindow__xpixels_to_col(width);
	const int rows = RgtWindow__ypixels_to_row(height);
	window->terminal = xterm__create(&window->child, title, cols, rows);
	
	if unlikely (!xterm__isvalid(&window->terminal)) {
		goto cleanup;
	}
	if unlikely (!FrameBuffer__init(&window->frame, (uint16_t)cols, (uint16_t)rows)) {
		goto cleanup;
	}
	
	return window;
cleanup:
	inner__cleanup_failed_create(window);
	return NULL;
}

void RgtWindow__destroy(RgtWindow *wnd)
{
	assert(wnd != NULL);
	assert(xterm__isvalid(&wnd->terminal));
	
	xterm__close(&wnd->terminal);
	FrameBuffer__destroy(&wnd->frame);
	rgt_free(wnd);
}

struct winsize RgtWindow__get_size(const RgtWindow *const wnd)
{
	assert(wnd != NULL);
	
	struct winsize ws = { 0 };
	ioctl(wnd->terminal.fd, TIOCGWINSZ, &ws);
	return ws;
}

void RgtWindow__draw_frame(const RgtWindow *const wnd)
{
	const struct FrameBuffer *const frame = &wnd->frame;
	xterm__write(&wnd->terminal, FrameBuffer__area(frame),
	             *frame->data);
	xterm__move_cursor_to(&wnd->terminal, 0, 0);
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
