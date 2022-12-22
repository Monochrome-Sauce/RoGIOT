#include "rogiot/rgt.h"
#include "src/macros.h"

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>


typedef struct {
	float x;
	float y;
} Vertex;


static struct RgtVertex vertShader(const void *_data)
{
	const Vertex *vert = _data;
	return (struct RgtVertex){
		.pos = {
			.x = vert->x,
			.y = vert->y,
			.z = 0.0f,
			.w = 1.0f,
		},
	};
}

int main(void)
{
	setlocale(LC_ALL, "en_US.UTF-8");
	RgtWindow *window = rgt__init(800, 600);
	assert(window != NULL);
	
	fprintf(stderr, "Initiated rgt: %s.\n", window != NULL ? "true" : "false");
	
	enum RgtError err = 0;
	do {
		err = rgt__error_pop();
		const char *const errName = rgt__error_name(err);
		const char *const errDesc = rgt__error_desc(err);
		
		assert(errName != NULL);
		assert(errDesc != NULL);
		fprintf(stderr, "\r(%d) %s: %s.\n", err, errName, errDesc);
	} while (err != RGT__E_OK);
	
	Vertex points[] = {
		{+0.60f, +0.4f}, {+0.60f, -0.6f},
		{-0.60f, +0.4f}, {-0.60f, -0.6f},
		{+0.30f, +0.2f}, {+0.30f, -0.25f},
		{-0.30f, +0.2f}, {-0.30f, -0.25f},
		{+0.15f, +0.1f}, {+0.15f, -0.05f},
		{-0.15f, +0.1f}, {-0.15f, -0.05f},
		
		{-0.9f, -1.0f}, {-0.15f, -0.05f},
		{+0.9f, -1.0f}, {+0.15f, -0.05f},
	};
	
	struct RgtDrawData data = {
		.vshader  = vertShader,
		.vertices = points,
		.count    = SIZEOF_ARRAY(points),
		.memSize  = sizeof (*points),
	};
	
	rgt__draw_2p(window, &data);
	rgt__swap_buffers(window);
	
	fputs("\nPress <ENTER> to exit... ", stderr);
	getc(stdin);
	
	
	rgt__deinit(window);
	return EXIT_SUCCESS;
}
