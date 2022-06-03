#include "BigInt.h"

#include <stdio.h>

// BiG Vector
typedef struct struct_vector {
    size_t size;
    size_t capacity;
    size_t *data;
} BGV;

typedef union {
    BGN *BGN;
    BGV *BGV;
} UN;

static inline void bg_append(BGV *vector, byte value) {
    if (vector->capacity != vector->size) {
        vector->data[vector->size++] = value;
    }else if (vector->capacity == 0) {
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

BGN *BGN_new_number() {
    return (BGN *) calloc(1, sizeof(BGV));
}

BGN *BGN_with_capacity(size_t capacity) {
    UN out;
    out.BGV = malloc(sizeof(BGV));
    *out.BGV = (BGV){
        .size = 0,
        .capacity = capacity,
        .positive = true
    };
}

void BI_delete(BGN *number) {
    UN temp;
    temp.BGN = number;
    free(temp.BGV->data);
    free(temp.BGV);
}




