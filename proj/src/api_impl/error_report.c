#include "../error_report.h"


static enum RgtError errorStack[RGT__ERROR_COUNT] = { RGT__E_OK };
static int errorCount = 0;

#define ASSSERT_ERROR_COUNT() (assert(0 <= errorCount && errorCount < RGT__ERROR_COUNT))


extern const char* rgt__error_name(const enum RgtError err)
{
#define CASE_RET(x) case x: return #x
	switch (err)
	{
		CASE_RET(RGT__E_OK);
		CASE_RET(RGT__E_SYSLIMIT);
		
		CASE_RET(RGT__ERROR_COUNT);
	}
	return NULL;
#undef CASE_RET
}

extern const char* rgt__error_desc(const enum RgtError err)
{
	switch (err)
	{
	case RGT__E_OK:
		return "Success";
	case RGT__E_SYSLIMIT:
		return "System limit on allocation of required resources";
	
	case RGT__ERROR_COUNT:
		return "Number of existing RoGIOT error codes";
	}
	return NULL;
}

extern enum RgtError rgt__error_pop(void)
{
	ASSSERT_ERROR_COUNT();
	
	if (errorCount == 0) {
		return RGT__E_OK;
	}
	return errorStack[--errorCount];
}

void rgt__error_push(const enum RgtError err)
{
	ASSSERT_ERROR_COUNT();
	assert(err != RGT__E_OK && err != RGT__ERROR_COUNT);
	
	errorStack[errorCount] = err;
	++errorCount;
}

void rgt__error_clear_all(void)
{
	ASSSERT_ERROR_COUNT();
	errorCount = 0;
}
