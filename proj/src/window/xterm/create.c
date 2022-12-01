#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#include "../xterm.h"
#include "src/macros.h"

#include <stdlib.h>    // pid_t, grantpt(), unlockpt(), ptsname(), abort()
#include <limits.h>    // PATH_MAX
#include <assert.h>

#include <unistd.h>    // pid_t, fork(), close(), execlp()
#include <fcntl.h>     // pid_t, open()


static int inner__create_pt_master(void)
{
	const int fdMaster = open("/dev/ptmx", O_RDWR | O_NOCTTY);
	if likely (fdMaster >= 0) {
		if unlikely (grantpt(fdMaster) < 0) {
			close(fdMaster);
			return -1;
		}
		
		if unlikely (unlockpt(fdMaster) < 0) {
			close(fdMaster);
			return -1;
		}
	}
	return fdMaster;
}

/* #Clear the STDIN and STDOUT of a newly created xterm terminal.
! Reason: some weird number (ending with a newline) is always being left in the
input and output of the terminal.
*/
static void inner__clear_xterm_io(const struct XTerm *x)
{
	/* clear stdin */
	char buff[128] = { 0 };
	xterm__read(x, sizeof (buff), buff);
	
	/* clear stdout */
	xterm__clear_screen(x);
	xterm__hide_cursor(x);
}

static struct XTerm inner__create_invalid(void)
{
	return (struct XTerm){ .fd = -1 };
}


struct XTerm xterm__create(pid_t *const childPid, const char title[], const int columns, const int rows)
{
	assert(childPid != NULL);
	assert(columns > 0 && rows > 0);
	
	const int fdMaster = inner__create_pt_master();
	
	const char *const slaveName = ptsname(fdMaster);
	if unlikely (slaveName == NULL) {
		return inner__create_invalid();
	}
	const int fdSlave = open(slaveName, O_RDWR | O_NOCTTY);
	
	*childPid = fork();
	if unlikely (*childPid < 0) {
		return inner__create_invalid();
	}
	
	
	if (*childPid > 0) { /* parent */
		close(fdMaster);
		
		struct XTerm result = { .fd = fdSlave };
		inner__clear_xterm_io(&result);
		return result;
	}
	else { /* child */
		close(fdSlave);
		
		char xtermOption_Sccn[PATH_MAX];
		snprintf(xtermOption_Sccn, sizeof (xtermOption_Sccn),
		         "-S%s/%d", slaveName, fdMaster);
		
		char xtermOption_font[sizeof (TOKSTR(XTERM_FONT_W) "x" TOKSTR(XTERM_FONT_H))];
		snprintf(xtermOption_font, sizeof (xtermOption_font),
		         "%dx%d", XTERM_FONT_W, XTERM_FONT_H);
		
		char xtermOption_geometry[32];
		snprintf(xtermOption_geometry, sizeof (xtermOption_geometry),
		         "%dx%d", columns, rows);
		
		execlp("xterm", "xterm", xtermOption_Sccn,
		       "-name", "rogiot",
		       "-font", xtermOption_font,
		       "-geom", xtermOption_geometry,
		       "-title", title == NULL ? "xterm" : title,
		       "+fullscreen", "+hold", "+j", "-aw",
		       (char*)NULL);
		close(fdMaster);
		abort(); /* abort the child process after xterm ends */
	}
}

void xterm__close(struct XTerm *x)
{
	assert(x != NULL && x->fd >= 0);
	close(x->fd);
	x->fd = -1;
}

bool xterm__isvalid(const struct XTerm *x)
{
	assert(x != NULL);
	return x->fd >= 0;
}
