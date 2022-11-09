#include "../rgt.h"
#include "Pipe.h"
#include "macros.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>


static struct
{
	pid_t childPid;
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


bool rgt__init(void)
{
	memset(&g_parent, 0, sizeof(g_parent));
	
	int parentToChild[2] = { 0 };
	if (unlikely(pipe(parentToChild) < 0)) {
		return false;
	}
	
	int childToParent[2] = { 0 };
	if (unlikely(pipe(childToParent) < 0)) {
		CLOSE_FD_ARRAY(parentToChild);
		return false;
	}
	
	g_parent.pipe.in = childToParent[PIPE_FD_IN];
	g_parent.pipe.out = parentToChild[PIPE_FD_OUT];
	struct Pipe childPipe = {
		.in = parentToChild[PIPE_FD_IN],
		.out = childToParent[PIPE_FD_OUT],
	};
	
	
	g_parent.childPid = fork();
	if (unlikely(g_parent.childPid < 0)) {
		CLOSE_FD_ARRAY(parentToChild);
		CLOSE_FD_ARRAY(childToParent);
		return false;
	}
	else if (g_parent.childPid > 0) { /* father */
		inner__close_pipe(&childPipe);
		return true;
	}
	else { /* child */
		inner__close_pipe(&g_parent.pipe);
		/* child process: execute it and pass it the pipe */
		exit(EXIT_SUCCESS);
	}
}

/* the child process may not call this function */
void rgt__deinit(void)
{
	assert(g_parent.childPid > 0);
	
	/* child process: inform it to finish */
	
	int status = 0;
	waitpid(g_parent.childPid, &status, WNOHANG);
	inner__close_pipe(&g_parent.pipe);
}

FILE* rgt__create_debug_output(const char *title)
{
	int fds[2] = { 0 };
	if(pipe(fds) == -1) {
		return NULL;
	}
	
	const pid_t child_pid = fork();
	if (unlikely(child_pid < 0)) {
		CLOSE_FD_ARRAY(fds);
		return NULL;
	}
	else if (child_pid == 0) { /* child */
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
	
	close(fds[PIPE_FD_IN]);
	FILE *const stream = fdopen(fds[PIPE_FD_OUT], "w");
	if (stream != NULL) {
		setvbuf(stream, NULL, _IOLBF, 0);
	}
	return stream;
}
