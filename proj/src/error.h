#ifndef ERROR_H
#define ERROR_H

#include "rogiot/rgt.h"


/* #Add an error for users of the library to see.
! Calling this function will push a new error to the stack of errors.
! @param err: valid enum RgtError value, may not be RGT__E_NONE or RGT__ERROR_COUNT.
*/
extern void rgt__error_push(enum RgtError err);

/* #Clear the stack of errors' contents, resetting the number of errors to 0.
*/
extern void rgt__error_clear_all(void);


#endif /* ERROR_H */
