#include "rogiot/rgt.h"

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


int main(void)
{
	FILE* xterm = rgt__create_debug_output("good morning world!!!!");
	assert(xterm != NULL);
	
	bool success = rgt__init();
	assert(success);
	
	int written = fprintf(xterm, "Initiated rgt: %s.\n", success ? "true" : "false");
	assert(written > 0);
	
	
	sleep(2);
	fclose(xterm);
	rgt__deinit();
	return EXIT_SUCCESS;
}
