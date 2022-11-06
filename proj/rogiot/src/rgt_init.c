#include "../rgt.h"
#include "Pipe.h"
#include "macros.h"

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>


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


static void inner__close_fds(size_t n, int fds[n])
{
	for (size_t i = 0; i < n; ++n)
	{
		close(fds[n]);
		fds[n] = -1;
	}
}

#define CLOSE_FD_ARRAY(fds) inner__close_fds(SIZEOF_ARRAY(fds), fds)


bool rgt__init(void)
{
	memset(&g_parent, 0, sizeof(g_parent));
	
	int parentToChild[2] = { 0 };
	if (pipe(parentToChild) < 0) {
		return false;
	}
	
	int childToParent[2] = { 0 };
	if (pipe(childToParent) < 0) {
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
	if (g_parent.childPid < 0) {
		CLOSE_FD_ARRAY(parentToChild);
		CLOSE_FD_ARRAY(childToParent);
		return false;
	}
	else if (g_parent.childPid > 0) {
		inner__close_pipe(&g_parent.pipe);
		/* run the child process and pass it the pipe */
		exit(EXIT_SUCCESS);
	}
	else {
		inner__close_pipe(&childPipe);
		return true;
	}
}
