#include "../rgt.h"
#include "error.h"


static enum RgtError errorStack[RGT__ERROR_COUNT] = { RGT__E_NONE };
static int errorCount = 0;

#define ASSSERT_ERROR_COUNT() (assert(0 <= errorCount && errorCount < RGT__ERROR_COUNT))


extern enum RgtError rgt__error_pop(void)
{
	ASSSERT_ERROR_COUNT();
	
	if (errorCount == 0) {
		return RGT__E_NONE;
	}
	return errorStack[--errorCount];
}

void rgt__error_push(enum RgtError err)
{
	ASSSERT_ERROR_COUNT();
	assert(err != RGT__E_NONE && err != RGT__ERROR_COUNT);
	
	errorStack[errorCount] = err;
	++errorCount;
}

void rgt__error_clear_all(void)
{
	ASSSERT_ERROR_COUNT();
	errorCount = 0;
}
