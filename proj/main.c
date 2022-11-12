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
	
	enum RgtError err = 0;
	do {
		err = rgt__error_pop();
		const char *errName = rgt__error_name(err);
		const char *errDesc = rgt__error_desc(err);
		
		assert(errName != NULL);
		assert(errDesc != NULL);
		fprintf(xterm, "(%d) %s: %s.\n", err, errName, errDesc);
	} while (err != RGT__E_OK);
	
	
	
	sleep(8); /* wait to make xterm visible to humans */
	rgt__deinit();
	fclose(xterm);
	return EXIT_SUCCESS;
}
