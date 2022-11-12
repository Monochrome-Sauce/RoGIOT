#ifndef ROGIOT__RGT_H
#define ROGIOT__RGT_H

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>


enum RgtError
{
	RGT__E_OK = 0,
	
	/* maintain the maximum number of errors for implementation reasons */
	RGT__ERROR_COUNT,
};
static_assert(!RGT__E_OK, "truthy RGT__E_OK value");

/* #Get error registered by the library.
! Calling this function will return the latest error the library registered and
delete it from the stack of errors.
! The list of errors gets cleared and filled with new errors whenever an API call fails.
!
! @return: valid enum RgtError value, RGT__E_NONE when no errors are left.
*/
extern enum RgtError rgt__error_pop(void);

/* #Get name of an RGT error.
! @param err: valid RgtError.
! @return: constant string which may not be modified by the caller.
*/
extern const char* rgt__error_name(enum RgtError err);

/* #Get description of an RGT error.
! @param err: valid RgtError.
! @return: constant string which may not be modified by the caller.
*/
extern const char* rgt__error_desc(enum RgtError err);





/* #Initiates the library, stealing the standard streams (stdin, stdout, stderr).
! The standard streams are still usable, but for the library to work properly
please refrain from utilizing them and refer to rgt::create_debug_output() for
creating a debug-output console.
!
! Must be called only once.
*/
extern bool rgt__init(void);

/* #Clears the resources used by the library.
! The call may take time to finish, in the case where it has to wait for a
thread/process to finish.
!
! Must be called after rgt::init().
*/
extern void rgt__deinit(void);


/* #Creates an xterm terminal with write permissions only.
! To close the created terminal, just call fclose() on the returned stream.
! This will not work unless your system has xterm in the PATH variables.
!
! @param title: C-string title for the created console. Pass NULL for a default title.
! @return: Line-buffered write-only FILE* stream.
*/
extern FILE* rgt__create_debug_output(const char *title);


#endif /* ROGIOT__RGT_H */
