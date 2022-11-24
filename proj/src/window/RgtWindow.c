#include "./RgtWindow.h"
#include "src/window/create_xterm.h"
#include "src/macros.h"


RgtWindow* RgtWindow__create(const char *const title)
{
	RgtWindow *window = rgt_new(RgtWindow);
	if (window != NULL) {
		window->terminal = create_xterm(&window->child, title);
		
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
