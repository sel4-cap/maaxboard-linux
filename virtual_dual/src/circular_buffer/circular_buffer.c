#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <circular_buffer.h>

circular_buffer_t* circular_buffer_init(uintptr_t circular_buffer, size_t size, uintptr_t data_buffer) {
    circular_buffer_t *cb = (circular_buffer_t *)circular_buffer;
    cb->buffer = (char *)data_buffer;
    cb->max = size;
    circular_buffer_reset(cb);
    return cb;
}

void circular_buffer_free(circular_buffer_t *cb) {
    free(cb->buffer);
    free(cb);
}

void circular_buffer_reset(circular_buffer_t *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->full = false;
}

bool circular_buffer_full(circular_buffer_t *cb) {
    return cb->full;
}

bool circular_buffer_empty(circular_buffer_t *cb) {
    return (!cb->full && (cb->head == cb->tail));
}

void circular_buffer_put(circular_buffer_t *cb, char data) {
    cb->buffer[cb->head] = data;

    printf("cb head %c\n", cb->buffer[cb->head]);
    printf("cb tail %c\n", cb->buffer[cb->tail]);

    if(cb->full) {
        cb->tail = (cb->tail + 1) % cb->max;
    }

    cb->head = (cb->head + 1) % cb->max;
    cb->full = (cb->head == cb->tail);
}

char circular_buffer_get(circular_buffer_t *cb) {
    if (circular_buffer_empty(cb)) {
        // Buffer is empty
        return -1; // Or some error code or handle it as needed
    }

    int data = cb->buffer[cb->tail];
    cb->full = false;
    cb->tail = (cb->tail + 1) % cb->max;

    return data;
}

