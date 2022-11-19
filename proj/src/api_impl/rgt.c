#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"
#include "src/Pipe.h"
#include "src/macros.h"
#include "src/error_report.h"

#include <stdlib.h>
#include <memory.h>
#include <threads.h>

#include <sys/types.h>
#include <termios.h>
#include <unistd.h>


static struct termios g_originalTerminalState;
static bool g_rgtInitiated = false;

static void inner__close_fds(const size_t len, int fds[len])
{
	for (size_t i = 0; i < len; ++i)
	{
		close(fds[i]);
		fds[i] = -1;
	}
}


extern bool rgt__init(void)
{
	if (g_rgtInitiated) {
		return false;
	}
	g_rgtInitiated = true;
	
	
	if unlikely (tcgetattr(STDIN_FILENO, &g_originalTerminalState) < 0) {
		return false;
	}
	
	struct termios terminal = g_originalTerminalState;
	terminal.c_lflag &= ~(unsigned)(ECHO | ICANON);
	if unlikely (tcsetattr(STDIN_FILENO, TCSANOW, &terminal) < 0) {
		return false;
	}
	
	struct termios result;
	return tcgetattr(STDIN_FILENO, &result) == 0 && terminal.c_lflag == result.c_lflag;
}

extern void rgt__deinit(void)
{
	if (!g_rgtInitiated) {
		return;
	}
	g_rgtInitiated = false;
	
	tcsetattr(STDIN_FILENO, TCSANOW, &g_originalTerminalState);
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
