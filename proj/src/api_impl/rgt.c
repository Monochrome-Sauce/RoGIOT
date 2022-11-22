#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"
#include "src/macros.h"
#include "src/RgtWindow.h"

#include <memory.h>
#include <termios.h>


static bool inner__termios_eq(struct termios *rhs, struct termios *lhs)
{
	return rhs->c_iflag == lhs->c_iflag &&
	       rhs->c_oflag == lhs->c_oflag &&
	       rhs->c_cflag == lhs->c_cflag &&
	       rhs->c_lflag == lhs->c_lflag &&
	       memcmp(rhs->c_cc, lhs->c_cc, NCCS) == 0;
}

static bool inner__try_modify_terminal(const int terminalFd)
{
	struct termios termState;
	if unlikely (tcgetattr(terminalFd, &termState) < 0) {
		return false;
	}
	
	termState.c_lflag &= ~(unsigned)(ECHO | ICANON);
	if unlikely (tcsetattr(terminalFd, TCSANOW, &termState) < 0) {
		return false;
	}
	
	/* check if modification was successful */
	struct termios newState;
	return tcgetattr(terminalFd, &newState) == 0 && inner__termios_eq(&termState, &newState);
}

extern RgtWindow* rgt__init(void)
{
	RgtWindow *wnd = RgtWindow__create("Rgt window");
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
