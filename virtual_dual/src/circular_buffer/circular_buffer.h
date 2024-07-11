#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *buffer;     // pointer to the buffer memory
    size_t head;     // index of the head
    size_t tail;     // index of the tail
    size_t max;      // maximum number of elements
    bool full;       // flag to indicate if the buffer is full
    bool lock;
} circular_buffer_t;

// Function prototypes
circular_buffer_t* circular_buffer_init(uintptr_t circular_buffer, size_t size, uintptr_t data_buffer);
void circular_buffer_free(circular_buffer_t *cb);
void circular_buffer_reset(circular_buffer_t *cb);
bool circular_buffer_full(circular_buffer_t *cb);
bool circular_buffer_empty(circular_buffer_t *cb);
void circular_buffer_put(circular_buffer_t *cb, char data);
char circular_buffer_get(circular_buffer_t *cb);