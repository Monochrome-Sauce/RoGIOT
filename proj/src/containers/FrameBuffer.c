#include "FrameBuffer.h"
#include "src/macros.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


static uint32_t inner__calc_size(const uint16_t x, const uint16_t y)
{
	return (uint32_t)x * (uint32_t)y;
}

bool FrameBuffer__init(struct FrameBuffer *const buff, const uint16_t width, const uint16_t height)
{
	assert(buff != NULL);
	assert(width > 0 && height > 0);
	
	buff->width = width;
	buff->height = height;
	buff->capacity = 0;
	
	const uint32_t capacity = inner__calc_size(width, height);
	buff->data = rgt_newarr(capacity, FrameBuffer_DataType);
	
	if likely (buff->data != NULL) {
		memset(buff->data, ' ', capacity * sizeof (FrameBuffer_DataType));
		buff->capacity = capacity;
	}
	return buff->data != NULL;
}

void FrameBuffer__destroy(struct FrameBuffer *const buff)
{
	rgt_free(buff->data);
}

bool FrameBuffer__resize(struct FrameBuffer *const buff, const uint16_t width, const uint16_t height)
{
	assert(buff != NULL && buff->data != NULL);
	assert(width > 0 && height > 0);
	
	const uint32_t newCapacity = inner__calc_size(width, height);
	if (newCapacity > buff->capacity) {
		void *tmp = realloc(buff->data, newCapacity * sizeof (FrameBuffer_DataType));
		
		if unlikely (tmp == NULL) {
			return false;
		}
		buff->data = tmp;
		buff->capacity = newCapacity;
	}
	
	buff->width = width;
	buff->height = height;
	return true;
}

FrameBuffer_DataType* FrameBuffer__at(struct FrameBuffer *const buff,
                                      const uint16_t x, const uint16_t y)
{
	assert(buff != NULL && buff->data != NULL);
	assert(x < buff->width && y < buff->height);
	
	return &(*buff->data)[y * buff->width + x];
}
