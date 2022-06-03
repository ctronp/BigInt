// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt

#include "BigInt.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// Possible implementation of Splay Tree

// BiG Vector

typedef enum struct_positive {
    none = (uint8_t) 0,
    negative = (uint8_t) 1,
    positive = (uint8_t) 2,
} Positive;

typedef struct struct_vector {
    uintptr_t size;
    uintptr_t capacity;
    uintptr_t *data;
    uint8_t positive;
} BGV;

typedef union {
    BGN *BGN;
    BGV *BGV;
} UN;

static inline void bg_append(BGV *vector, byte value) {
    if (vector->capacity != vector->size) {
        vector->data[vector->size++] = value;
    } else if (vector->capacity == 0) {
        vector->capacity = 1;
        vector->data = malloc(sizeof(size_t));
        vector->data[0] = value;
    } else {
        vector->capacity <<= 2;
        vector->data = realloc(vector->data, vector->capacity);
        vector->data[vector->size++] = value;
    }
}

static inline size_t bg_byte_capacity(BGV *vector) {
    return vector->capacity * sizeof(size_t);
}

static inline size_t bg_byte_size(BGV *vector) {
    return vector->size * sizeof(size_t);
}

static inline BGN *bg_with_capacity(size_t capacity) {
    UN out;
    out.BGV = malloc(sizeof(BGV));
    *out.BGV = (BGV) {
            .size = 0,
            .capacity = capacity
    };
    return out.BGN;
}

BGN *BGN_new_number() {
    return (BGN *) calloc(1, sizeof(BGV));
}


void BGN_delete(BGN *number) {
    UN temp;
    temp.BGN = number;
    free(temp.BGV->data);
}


// Small Testing
static inline void compiler_assert() {
    assert(sizeof(Positive) == 1);

}




