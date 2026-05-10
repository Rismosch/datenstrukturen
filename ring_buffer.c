#include "ring_buffer.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct RingBuffer {
    uint32_t capacity;
    uint32_t len;
    uint32_t head;
    uint32_t tail;
    void **buf;
};

RingBuffer *ring_buffer_new(uint32_t capacity) {
    if (capacity == 0) {
        return false;
    }

    RingBuffer *ds;

    ds = malloc(sizeof(RingBuffer));

    ds->capacity = capacity;
    ds->len = 0;
    ds->head = 0;
    ds->tail = 0;
    ds->buf = malloc(sizeof(int32_t) * capacity);

    return ds;
}

void ring_buffer_delete(RingBuffer *ds) {
    if (!ds) {
        return;
    }

    free(ds->buf);
    free(ds);
}

uint32_t ring_buffer_capacity(RingBuffer *ds) {return ds->capacity;}
uint32_t ring_buffer_len(RingBuffer *ds) {return ds->len;}

bool ring_buffer_enque(RingBuffer *ds, void *x) {
    if (ds->len == ds->capacity) {
        return false;
    }

    ds->buf[ds->head] = x;
    ds->head = (ds->head + 1) % ds->capacity;
    ds->len += 1;

    return true;
}

bool ring_buffer_deque(RingBuffer* ds, void **x) {
    if (ds->len == 0) {
        return false;
    }

    *x = ds->buf[ds->tail];
    ds->tail = (ds->tail + 1) % ds->capacity;
    ds->len -= 1;

    return true;
}
