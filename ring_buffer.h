#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct RingBuffer RingBuffer;

RingBuffer *ring_buffer_new(uint32_t capacity);
void ring_buffer_delete(RingBuffer *ds);

uint32_t ring_buffer_capacity(RingBuffer* ds);
uint32_t ring_buffer_len(RingBuffer* ds);
bool ring_buffer_enque(RingBuffer* ds, void *x);
bool ring_buffer_deque(RingBuffer* ds, void **x);

#endif
