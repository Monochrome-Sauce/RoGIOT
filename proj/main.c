#include "rogiot/rgt.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int x = 0;
void print(void)
{
	printf("%d | %d\n", x, getpid());
}


int main(void)
{
	atexit(print);
	x = rgt__init();
}
