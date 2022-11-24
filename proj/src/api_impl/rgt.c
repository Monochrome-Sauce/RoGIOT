#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"
#include "src/macros.h"
#include "src/window/RgtWindow.h"

#include <stdlib.h>
#include <string.h>
#include <termios.h>


static bool inner__termios_eq(struct termios *rhs, struct termios *lhs)
{
	/* compare only the values POSIX guarentees to exist */
	return rhs->c_iflag == lhs->c_iflag &&
	       rhs->c_oflag == lhs->c_oflag &&
	       rhs->c_cflag == lhs->c_cflag &&
	       rhs->c_lflag == lhs->c_lflag &&
	       memcmp(rhs->c_cc, lhs->c_cc, NCCS) == 0;
}

static bool inner__try_modify_terminal(const int fdTerm)
{
	struct termios termState;
	if unlikely (tcgetattr(fdTerm, &termState) < 0) {
		return false;
	}
	
	termState.c_lflag &= ~(unsigned)(ECHO | ICANON);
	if unlikely (tcsetattr(fdTerm, TCSANOW, &termState) < 0) {
		return false;
	}
	
	/* check if modification was successful */
	struct termios newState;
	return tcgetattr(fdTerm, &newState) == 0 && inner__termios_eq(&termState, &newState);
}

extern RgtWindow* rgt__init(void)
{
	RgtWindow *wnd = RgtWindow__create("Rgt window", 800, 600);
	if unlikely (wnd == NULL) {
		return NULL;
	}
	
	if unlikely (!inner__try_modify_terminal(fileno(wnd->terminal))) {
		rgt_free(wnd);
	}
	return wnd;
}

extern void rgt__deinit(RgtWindow *const window)
{
	RgtWindow__destroy(window);
}
