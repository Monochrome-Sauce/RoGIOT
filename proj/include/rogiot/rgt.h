#ifndef ROGIOT__RGT_H
#define ROGIOT__RGT_H

#include <stdbool.h>
#include <assert.h>


enum RgtError
{
	RGT__E_OK = 0,
	RGT__E_SYSLIMIT,
	
	/* maintain the maximum number of errors for implementation reasons */
	RGT__ERROR_COUNT,
};
static_assert(!RGT__E_OK, "truthy RGT__E_OK value");

/* #Get error registered by the library.
! Calling this function will return the latest error the library registered and delete
it from the stack of errors.
! The list of errors may get cleared and filled with new errors with each API call.
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





typedef struct RgtWindow RgtWindow;

/* #Initiate the library, creating a new terminal window.
! @param width: initial width (in pixels) of the created window; must be positive.
! @param height: initial height (in pixels) of the created window; must be positive.
! @return: NULL if initialization failed, otherwise an RgtWindow object.
*/
extern RgtWindow* rgt__init(int width, int height);

/* #Clears the resources used by a created RgtWindow object.
! @window: result of an rgt::init() call. Must NOT be NULL.
*/
extern void rgt__deinit(RgtWindow *window);





struct RgtVertex {
	union {
		float v[4];
		struct {
			float x, y, z, w;
		} pos;
		struct {
			float r, g, b, a;
		} col;
	};
};

typedef struct RgtVertex (RgtFunc_VertexShader)(const void*);

struct RgtDrawData {
	/* user function which evaluates every member of 'vertices' into an RgtVertex structure */
	RgtFunc_VertexShader *vshader;
	
	const void *vertices; /* array of user objects, each member is passed to 'vshader()' */
	unsigned int count; /* number of members 'vertices' points at */
	unsigned int memSize; /* size in bytes of each member in 'vertices' */
};


/* #Draw lines between 2 points.
! @param wnd: the window to draw to.
! @param drawData: a structure containing the data necessary for drawing.
*/
extern void rgt__draw_2p(RgtWindow *wnd, const struct RgtDrawData *drawData);

/* #Flush the data to the window. Uses a new buffer which should be cleared with rgt__clearempty.
! @param wnd: the window to draw on.
*/
extern void rgt__swap_buffers(RgtWindow *window);


#endif /* ROGIOT__RGT_H */
