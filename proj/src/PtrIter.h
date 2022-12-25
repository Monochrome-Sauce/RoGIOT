#ifndef PTR_ITER_H
#define PTR_ITER_H

#include <stddef.h>

/*
The iterators are used to simplify generic byte iteration.
PtrIter and ConstPtrIter are identical except for const-correctness.
They contain only 2 fields:
	[Const]PtrIter::ptr    - generic pointer to '[const] void'.
	[Const]PtrIter::stride - the size in bytes of the object pointed at.

PtrIter__ADD() - iterate forwards (N * iter.stride) bytes.
PtrIter__SUB() - iterate backwards (N * iter.stride) bytes.
PtrIter__INC() - iterate forwards (1 * iter.stride) bytes.
PtrIter__DEC() - iterate backwards (1 * iter.stride) bytes.

PtrIter__COPY_AT() - copy an existing iterator at a forward offset of (N * iter.stride) bytes.
*/

struct PtrIter {
	void *ptr;
	ptrdiff_t stride;
};

struct ConstPtrIter {
	const void *ptr;
	ptrdiff_t stride;
};


#define PtrIter__ADD(iter, n) \
	do { \
		const unsigned char *const p = (iter).ptr;\
		(iter).ptr = p + (n * (iter).stride); \
	} while (0)

#define PtrIter__SUB(iter, n) PtrIter__ADD((iter), -(n))

#define PtrIter__INC(iter) PtrIter__ADD((iter), 1)

#define PtrIter__DEC(iter) PtrIter__ADD((iter), -1)

#define PtrIter__COPY_AT(iter, n) (_Generic(iter, \
	struct PtrIter: (struct PtrIter)PtrIter__((iter), (n)), \
	struct ConstPtrIter: (struct ConstPtrIter)PtrIter__((iter), (n))))

#define PtrIter__(iter, n) \
	{ \
		.ptr = ((char*)(iter).ptr) + (n) * (iter).stride, \
		.stride = (iter).stride, \
	}


#endif /* PTR_ITER_H */
