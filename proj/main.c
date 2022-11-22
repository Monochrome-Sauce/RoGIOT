#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>


int main(void)
{
	pid_t child = 0;
	FILE *const xterm = rgt__create_xterm(&child, "Tester debug");
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
		fprintf(xterm, "\r(%d) %s: %s.\n", err, errName, errDesc);
	} while (err != RGT__E_OK);
	
	fputs("Press <ENTER> to exit... ", xterm);
	getc(xterm);
	
	
	rgt__deinit();
	fclose(xterm);
	return EXIT_SUCCESS;
}
