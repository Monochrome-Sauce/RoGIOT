#include "rogiot/rgt.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <unistd.h>


int main(void)
{
	FILE *const xterm = rgt__create_debug_output("Tester debug");
	assert(xterm != NULL);
	
	bool success = rgt__init();
	assert(success);
	
	int written = fprintf(xterm, "Initiated rgt: %s.\n", success ? "true" : "false");
	assert(written > 0);
	
	const char *errName = rgt__error_name(RGT__E_OK);
	const char *errDesc = rgt__error_desc(RGT__E_OK);
	assert(errName != NULL);
	assert(errDesc != NULL);
	fprintf(xterm, "%s: %s.\n", errName, errDesc);
	
	
	sleep(3); /* wait so we can see xterm */
	rgt__deinit();
	fclose(xterm);
	return EXIT_SUCCESS;
}
