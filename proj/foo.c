#include <stdio.h>



int print_hello_world(FILE* stream)
{
	return fprintf(stream, "Hello world!\n");
}
