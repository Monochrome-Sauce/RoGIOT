#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"
#include "Pipe.h"
#include "macros.h"

#include <stdlib.h>
#include <memory.h>
#include <threads.h>

#include <sys/types.h>
#include <unistd.h>


static struct
{
	thrd_t childThread;
	struct Pipe pipe;
} g_parent;


static void inner__close_pipe(struct Pipe *pipe)
{
	close(pipe->in);
	pipe->in = -1;
	
	close(pipe->out);
	pipe->out = -1;
}


static void inner__close_fds(const size_t len, int fds[len])
{
	for (size_t i = 0; i < len; ++i)
	{
		close(fds[i]);
		fds[i] = -1;
	}
}

#define CLOSE_FD_ARRAY(fds) inner__close_fds(SIZEOF_ARRAY(fds), fds)


/* #the child's threads and library's main.
! @param comms: Bi-directional pipe to communicate with the calling thread.
! @param term: Bi-directional pipe to communicate with the terminal.
*/
extern int mainloop(const struct Pipe comms, const struct Pipe term);

static int start_mainloop(void *data)
{
	struct Pipe *comms = data;
	struct Pipe term = {
		.in = STDIN_FILENO,
		.out = STDOUT_FILENO,
	};
	return mainloop(*comms, term);
}



extern bool rgt__init(void)
{
	memset(&g_parent, 0, sizeof(g_parent));
	
	int parentToChild[2] = { 0 };
	if unlikely (pipe(parentToChild) < 0) {
		return false;
	}
	
	int childToParent[2] = { 0 };
	if unlikely (pipe(childToParent) < 0) {
		CLOSE_FD_ARRAY(parentToChild);
		return false;
	}
	
	g_parent.pipe.in = childToParent[PIPE_FD_IN];
	g_parent.pipe.out = parentToChild[PIPE_FD_OUT];
	
	/* the variable is static so it won't go out of scope before the thread copies its data */
	static struct Pipe childPipe = { 0 };
	childPipe.in = parentToChild[PIPE_FD_IN];
	childPipe.out = childToParent[PIPE_FD_OUT];
	
	const int res = thrd_create(&g_parent.childThread, start_mainloop, &childPipe);
	return res == 0;
}

extern void rgt__deinit(void)
{
	/* child process: inform it to finish */
	const ssize_t res = write(g_parent.pipe.out, "Q", 1);
	printf("write: %zd\n", res);
	
	thrd_join(g_parent.childThread, NULL);
	inner__close_pipe(&g_parent.pipe);
}

extern FILE* rgt__create_debug_output(const char *title)
{
	int fds[2] = { 0 };
	if(pipe(fds) == -1) {
		return NULL;
	}
	
	const pid_t childPid = fork();
	if unlikely (childPid < 0) {
		CLOSE_FD_ARRAY(fds);
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
		       "-e", "cat", pipeFilepath,
		       (char*)NULL
		);
		abort(); /* abort the child process after xterm finishes */
	}
}
