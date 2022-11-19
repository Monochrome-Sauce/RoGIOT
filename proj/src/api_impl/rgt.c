#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"
#include "src/Pipe.h"
#include "src/macros.h"
#include "src/error_report.h"

#include <termios.h>
#include <unistd.h>


static struct termios g_originalTerminalState;
static bool g_rgtInitiated = false;

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
