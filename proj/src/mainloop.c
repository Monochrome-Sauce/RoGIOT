#include "rogiot/rgt.h"
#include "Pipe.h"
#include "macros.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <poll.h>


/* #Check for validty of pipes passed to mainloop().
*/
static bool are_pipes_valid(const struct Pipe *const comms, const struct Pipe *const term)
{
	struct pollfd toPoll[] = {
		{ .fd = comms->in,  .events = POLLNVAL, },
		{ .fd = comms->out, .events = POLLNVAL, },
		{ .fd = term->in,   .events = POLLNVAL, },
		{ .fd = term->out,  .events = POLLNVAL, },
	};
	
	if unlikely (poll(toPoll, SIZEOF_ARRAY(toPoll), 0) < 0) {
		perror("Failed to do initial poll");
		return false;
	}
	else {
		for (size_t i = 0; i < SIZEOF_ARRAY(toPoll); ++i)
		{
			if (toPoll[i].revents & POLLNVAL) {
				fprintf(stderr, "Pipe %d has an invalid file descriptor.", toPoll[i].fd);
				return false;
			}
		}
	}
	
	return true;
}

int mainloop(const struct Pipe comms, const struct Pipe term)
{
	if (!are_pipes_valid(&comms, &term)) {
		return EXIT_FAILURE;
	}
	
	FILE *const xterm = rgt__create_debug_output("RoGIOT mainloop() debug");
	if (xterm == NULL) {
		perror("Failed to create a debug window");
		return EXIT_FAILURE;
	}
	
	enum {
		TOPOLL_COMMS,
		TOPOLL_TERM,
	};
	struct pollfd toPoll[] = {
		[TOPOLL_COMMS] = { .fd = comms.in, .events = POLLIN, },
		[TOPOLL_TERM] = { .fd = term.in, .events = POLLIN, },
	};
	
	while (true)
	{
		if unlikely (poll(toPoll, SIZEOF_ARRAY(toPoll), -1) < 0) {
			fprintf(xterm, "Failed to poll: %s", strerror(errno));
			return EXIT_FAILURE;
		}
		
		if (fputs("Hello world\n", stdout) < 0) {
			perror("Well... Fuck");
		}
		break;
	}
	
	fclose(xterm);
	return EXIT_SUCCESS;
}
