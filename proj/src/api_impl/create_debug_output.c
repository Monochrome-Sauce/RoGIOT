#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"
#include "src/macros.h"
#include "src/error_report.h"
#include "src/Pipe.h"

#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>


static void inner__close_fds(const size_t len, int fds[len])
{
	for (size_t i = 0; i < len; ++i)
	{
		close(fds[i]);
		fds[i] = -1;
	}
}

extern FILE* rgt__create_debug_output(const char *title)
{
	int fds[2] = { 0 };
	if unlikely (pipe(fds) == -1) {
		return NULL;
	}
	
	const pid_t childPid = fork();
	if unlikely (childPid < 0) {
		inner__close_fds(SIZEOF_ARRAY(fds), fds);
		
		rgt__error_clear_all();
		rgt__error_push(RGT__E_SYSLIMIT);
		return NULL;
	}
	else if (childPid > 0) { /* parent */
		close(fds[PIPE_FD_IN]);
		
		FILE *const stream = fdopen(fds[PIPE_FD_OUT], "w");
		if (stream != NULL) {
			setvbuf(stream, NULL, _IOLBF, 0);
		}
		return stream;
	}
	else { /* child */
		close(fds[PIPE_FD_OUT]);
		
		char pipeFilepath[64] = { 0 };
		snprintf(pipeFilepath, sizeof(pipeFilepath), "/dev/fd/%d", fds[PIPE_FD_IN]);
		
		if (title == NULL) {
			title = "rgt debug output";
		}
		
		execlp("xterm", "xterm",
		       "-title", title,
		       "-e", "cat", pipeFilepath, "--number",
		       (char*)NULL
		);
		abort(); /* abort the child process after xterm finishes */
	}
}
