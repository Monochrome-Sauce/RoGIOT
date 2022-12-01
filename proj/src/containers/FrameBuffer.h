#ifndef CONTAINERS__FRAME_BUFFER_H
#define CONTAINERS__FRAME_BUFFER_H

#include <stdbool.h>
#include <stdint.h>


typedef char FrameBuffer_DataType;
struct FrameBuffer
{
	uint32_t capacity; /* the real amount of allocated data members */
	uint16_t width, height; /* imaginary xy dimensions for access */
	FrameBuffer_DataType (*data)[];
};


/* #Initialize a the buffer's data and sets all members to a space character.
! @param buff: pointer to a FrameBuffer.
! @return: false on failure to initiate the data buffer, otherwise true. When false is
returned, the width and height will be initiated to 0.
*/
bool FrameBuffer__init(struct FrameBuffer *const buff, uint16_t width, uint16_t height);

/* #Free an object's data.
! @param buff: pointer to a FrameBuffer initiated with FrameBuffer::init().
*/
void FrameBuffer__destroy(struct FrameBuffer *buff);

/* #Resize an existing object's data.
! @param buff: pointer to a FrameBuffer.
! @return: false on failure to resize, otherwise true.
*/
bool FrameBuffer__resize(struct FrameBuffer *buff, uint16_t width, uint16_t height);

/* #Get pointer to an element of the buffer.
! @param buff: pointer to a FrameBuffer.
! @param x: X position of element. X must be less than the width of the buffer.
! @param y: Y position of element. Y must be less than the height of the buffer.
! @return: pointer to the element of the buffer at position (y,x).
*/
FrameBuffer_DataType* FrameBuffer__at(struct FrameBuffer *buff, uint16_t x, uint16_t y);

/* #Get the accessable area of the buffer.
! @param buff: pointer to a FrameBuffer.
! @return: product of the width and height of the buffer.
*/
uint32_t FrameBuffer__area(const struct FrameBuffer *buff);


#endif /* CONTAINERS__FRAME_BUFFER_H */
