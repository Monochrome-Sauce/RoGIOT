#include "rogiot/rgt.h"
#include "src/window/RgtWindow.h"
#include "src/macros.h"
#include "src/coroutine.h"

#include <stdlib.h>
#include <stddef.h>
#include <math.h>



typedef struct {
	int x, y;
} RgtPoint_i;

typedef struct {
	int _co__state;
	
	int _deltaX, _deltaY;
	int _signX, _signY;
	int _error;
	
	RgtPoint_i p;
} Ctx_point_generator;


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

static void inner__point_generator(Ctx_point_generator *const ctx, const RgtPoint_i final)
{
	RgtPoint_i *const p = &ctx->p;
	CO__START(*ctx);
	
	ctx->_deltaX = abs(final.x - p->x);
	ctx->_deltaY = -abs(final.y - p->y);
	ctx->_signX = final.x > p->x ? 1 : -1;
	ctx->_signY = final.y > p->y ? 1 : -1;
	ctx->_error = ctx->_deltaX + ctx->_deltaY;
	
	while (true)
	{
		CO__YIELD(*ctx);
		if (p->x == final.x && p->y == final.y) { 
			CO__FINISH();
		};
		
		const int e2 = 2 * ctx->_error;
		if (e2 >= ctx->_deltaY) {
			if (p->x == final.x) {
				CO__FINISH();
			}
			ctx->_error += ctx->_deltaY;
			p->x += ctx->_signX;
		}
		
		if (e2 <= ctx->_deltaX) {
			if (p->y == final.y) {
				CO__FINISH();
			}
			ctx->_error += ctx->_deltaX;
			p->y += ctx->_signY;
		}
	}
	
	CO__END(*ctx);
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

extern void rgt__draw_lines(RgtWindow *const wnd, const struct RgtDrawData *const drawData)
{
	assert(drawData->vshader != NULL);
	assert(drawData->count % 2 == 0);
	
	const char *currVertex = drawData->vertices;
	const char *const endVertex = currVertex + ((size_t)drawData->count * drawData->memSize);
	const ptrdiff_t step = drawData->memSize;
	
	while (currVertex != endVertex)
	{
		RgtPoint_i p[2] = { 0 };
		for (unsigned int i = 0; i < SIZEOF_ARRAY(p); ++i)
		{
			struct RgtVertex v = drawData->vshader(currVertex);
			inner__normalize_vertex(&v);
			p[i].x = inner__real_to_display(v.pos.x, wnd->frame.width);
			p[i].y = inner__real_to_display(v.pos.y, wnd->frame.height);
			currVertex += step;
		}
		
		Ctx_point_generator ctx = { .p = p[0] };
		CO__INIT_CTX(ctx);
		do {
			inner__point_generator(&ctx, p[1]);
			inner__plot_point(&ctx.p, &wnd->frame, '#');
		} while (!CO__IS_FINISHED(ctx));
	}
}

extern void rgt__swap_buffers(RgtWindow *const wnd)
{
	RgtWindow__draw_frame(wnd);
}
