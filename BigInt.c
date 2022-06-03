// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt

#include "BigInt.h"

#include <stdlib.h>
#include <stdbool.h>

// Possible implementation of Splay Tree

// BiG Vector

typedef enum __attribute__((__packed__)) struct_positive {
    none = 0,
    negative,
    positive
} Positive;

typedef struct struct_vector {
    size_t size;
    size_t capacity;
    size_t *data;
    Positive positive;
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


// coding time test based on the IDE,
// the IDE warn what is not going to be run.
__attribute__((unused)) static inline void compiler_assert() {
    if (sizeof(Positive) == 1) exit(1);


    exit(0);
}




