#include "rogiot/rgt.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <unistd.h>


int main(void)
{
	FILE *xterm = rgt__create_debug_output("good morning world!!!!");
	assert(xterm != NULL);
	
	bool success = rgt__init();
	assert(success);
	
	int written = fprintf(xterm, "Initiated rgt: %s.\n", success ? "true" : "false");
	assert(written > 0);
	
	sleep(3); /* have xterm properly show up */
	
	rgt__deinit();
	fclose(xterm);
	return EXIT_SUCCESS;
}
