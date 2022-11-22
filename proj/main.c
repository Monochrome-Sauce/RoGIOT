#define _POSIX_C_SOURCE 1
#include "rogiot/rgt.h"

#include <stdlib.h>
#include <stdio.h>


int main(void)
{
	RgtWindow *window = rgt__init();
	assert(window != NULL);
	
	fprintf(stderr, "Initiated rgt: %s.\n", window != NULL ? "true" : "false");
	
	enum RgtError err = 0;
	do {
		err = rgt__error_pop();
		const char *const errName = rgt__error_name(err);
		const char *const errDesc = rgt__error_desc(err);
		
		assert(errName != NULL);
		assert(errDesc != NULL);
		fprintf(stderr, "\r(%d) %s: %s.\n", err, errName, errDesc);
	} while (err != RGT__E_OK);
	
	fputs("\nPress <ENTER> to exit... ", stderr);
	getc(stdin);
	
	
	rgt__deinit(window);
	return EXIT_SUCCESS;
}
