#include "../rgt.h"

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>


union PipeFds
{
	struct {
		int in;
		int out;
	};
	int fds[2];
};


struct RgtProcess
{
	pid_t child;
	int pipeOut;
};
static struct RgtProcess g_proc;


bool rgt__init(void)
{
	memset(&g_proc, 0, sizeof(g_proc));
	
	union PipeFds p;
	if (pipe(p.fds) < 0) {
		return false;
	}
	
	g_proc.child = fork();
	if (g_proc.child < 0) {
		close(p.out);
		close(p.in);
		return false;
	}
	else if (g_proc.child > 0) {
		close(p.out);
		/* run the child process and pass it p.in */
		exit(EXIT_SUCCESS);
	}
	else {
		close(p.in);
		return true;
	}
}
