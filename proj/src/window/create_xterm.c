#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#include "src/window/create_xterm.h"
#include "src/macros.h"

#include <stdlib.h>    // pid_t, grantpt(), unlockpt(), ptsname(), abort()
#include <stdio.h>     // FILE*, fdopen(), perror()
#include <limits.h>    // PATH_MAX

#include <unistd.h>    // pid_t, fork(), close(), execlp()
#include <fcntl.h>     // pid_t, open()


static int create_pt_master(void)
{
	const int fdMaster = open("/dev/ptmx", O_RDWR | O_NOCTTY);
	if (fdMaster >= 0) {
		if (grantpt(fdMaster) < 0) {
			perror("grantpt()");
			close(fdMaster);
			return -1;
		}
		
		if (unlockpt(fdMaster) < 0) {
			perror("unlockpt()");
			close(fdMaster);
			return -1;
		}
	}
	else {
		perror("open(/dev/ptmx, ...)");
	}
	
	return fdMaster;
}

/* #Clear the STDIN and STDOUT of a newly created xterm terminal.
! Reason: some weird number (ending with a newline) is always being left in the
input and output of the terminal
*/
static void inner__clear_xterm(FILE *const xterm)
{
	/* clear stdin */
	char buff[64] = { 0 };
	char *res = fgets(buff, sizeof (buff), xterm);
	(void)res;
	
	/* clear stdout */
	fputs("\033c", xterm);
	fflush(xterm);
}


extern FILE* create_xterm(pid_t *const childPid, const char *const title)
{
	const int fdMaster = create_pt_master();
	
	const char *const slaveName = ptsname(fdMaster);
	if unlikely (slaveName == NULL) {
		return NULL;
	}
	const int fdSlave = open(slaveName, O_RDWR | O_NOCTTY);
	
	*childPid = fork();
	if unlikely (*childPid < 0) {
		return NULL;
	}
	
	
	if (*childPid > 0) { /* parent */
		close(fdMaster);
		
		FILE *const stream = fdopen(fdSlave, "r+");
		if likely (stream != NULL) {
			inner__clear_xterm(stream);
		}
		else {
			close(fdSlave);
		}
		
		return stream;
	}
	else { /* child */
		close(fdSlave);
		
		char xtermOption_Sccn[PATH_MAX] = { 0 };
		snprintf(xtermOption_Sccn, sizeof (xtermOption_Sccn), "-S%s/%d", slaveName, fdMaster);
		
		execlp("xterm", "xterm", xtermOption_Sccn,
		       "-title", title == NULL ? "xterm" : title,
		       (char*)NULL);
		close(fdMaster);
		abort(); /* abort the child process after xterm ends */
	}
}
