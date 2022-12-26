#include "rogiot/rgt.h"
#include "src/window/RgtWindow.h"
#include "src/macros.h"
#include "src/coroutine.h"
#include "src/PtrIter.h"

#include <stdlib.h>
#include <math.h>



typedef struct {
	int x, y;
} RgtPoint_i;

struct inner__generate_point {
	int _co__state;
	
	int m_deltaX, m_deltaY;
	int m_signX, m_signY;
	int m_error;
	
	RgtPoint_i _begin;
	RgtPoint_i _end;
};


/*void plot_line(RgtPoint_i p, const RgtPoint_i final)
{
	const int deltaX = abs(final.x - p.x);
	const int signX = final.x > p.x ? 1 : -1;
	
	const int deltaY = -abs(final.y - p.y);
	const int signY = final.y > p.y ? 1 : -1;
	
	int error = deltaX + deltaY;
	while (true)
	{
		plotXY(p.x, p.y)
		if (p.x == final.x && p.y == final.y) { break; };
		
		const int e2 = 2 * error;
		if (e2 >= deltaY) {
			if (p.x == final.x) { break; }
			error += deltaY;
			p.x += signX;
		}
		
		if (e2 <= deltaX) {
			if (p.y == final.y) { break; }
			error += deltaX;
			p.y += signY;
		}
	}
}*/

static RgtPoint_i inner__generate_point(struct inner__generate_point *const ctx)
{
	CO__START(*ctx);
	
	ctx->m_deltaX = abs(ctx->_end.x - ctx->_begin.x);
	ctx->m_deltaY = -abs(ctx->_end.y - ctx->_begin.y);
	ctx->m_signX = ctx->_end.x > ctx->_begin.x ? 1 : -1;
	ctx->m_signY = ctx->_end.y > ctx->_begin.y ? 1 : -1;
	ctx->m_error = ctx->m_deltaX + ctx->m_deltaY;
	
	while (true)
	{
		CO__YIELD(*ctx, ctx->_begin);
		if (ctx->_begin.x == ctx->_end.x && ctx->_begin.y == ctx->_end.y) { 
			CO__FINISH();
		};
		
		const int e2 = 2 * ctx->m_error;
		if (e2 >= ctx->m_deltaY) {
			if (ctx->_begin.x == ctx->_end.x) {
				CO__FINISH();
			}
			ctx->m_error += ctx->m_deltaY;
			ctx->_begin.x += ctx->m_signX;
		}
		
		if (e2 <= ctx->m_deltaX) {
			if (ctx->_begin.y == ctx->_end.y) {
				CO__FINISH();
			}
			ctx->m_error += ctx->m_deltaX;
			ctx->_begin.y += ctx->m_signY;
		}
	}
	
	CO__END(*ctx);
	return ctx->_begin;
}

static void inner__plot_point(const RgtPoint_i *p, struct FrameBuffer *buff,
	const FrameBuffer_DataType c)
{
	const int x = p->x;
	const int y = buff->height - p->y - 1;
	
	if (CAN_INDEX(x, buff->width) && CAN_INDEX(y, buff->height)) {
		*FrameBuffer__at(buff, (uint16_t)x, (uint16_t)y) = c;
	}
}

static int inner__real_to_display(float real, int display)
{
	return (int)roundf((float)display * (real + 1.0f) / 2);
}

static void inner__normalize_vertex(struct RgtVertex *const vertex)
{
	const float w = vertex->v[3];
	for (unsigned int i = 0; i < SIZEOF_ARRAY(vertex->v); ++i)
	{
		vertex->v[i] /= w;
	}
}

extern void rgt__draw_lines(
	RgtWindow *const wnd,
	const struct RgtArrayObject *const array,
	const struct RgtShaderFuncs *const shader
) {
	assert(array != NULL && shader != NULL);
	assert(array->vertices != NULL && shader->vertex != NULL);
	
	enum { POINT_COUNT = 2 };
	
	const ptrdiff_t count = array->length - (array->length % POINT_COUNT);
	struct ConstPtrIter iter = { array->vertices, array->memSize };
	const struct ConstPtrIter end = PtrIter__COPY_AT(iter, count);
	
	while (iter.ptr < end.ptr)
	{
		RgtPoint_i p[POINT_COUNT] = { 0 };
		for (int i = 0; i < POINT_COUNT; ++i)
		{
			struct RgtVertex v = shader->vertex(iter.ptr);
			inner__normalize_vertex(&v);
			p[i].x = inner__real_to_display(v.pos.x, wnd->frame.width);
			p[i].y = inner__real_to_display(v.pos.y, wnd->frame.height);
			PtrIter__INC(iter);
		}
		
		struct inner__generate_point ctx = {
			._begin = p[0],
			._end = p[1]
		};
		CO__INIT_CTX(ctx);
		do {
			RgtPoint_i point = inner__generate_point(&ctx);
			inner__plot_point(&point, &wnd->frame, '#');
		} while (!CO__IS_FINISHED(ctx));
	}
}

extern void rgt__swap_buffers(RgtWindow *const wnd)
{
	RgtWindow__draw_frame(wnd);
}
