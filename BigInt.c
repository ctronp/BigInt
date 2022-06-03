// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt

#include "BigInt.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// Possible implementation of Splay Tree

// BiG Vector

typedef enum struct_positive {
    positive_zero = 0,
    negative = -2,
} Positive;

typedef struct struct_vector {
    uintmax_t size;
    uintmax_t capacity;
    uintmax_t *data;
    int8_t positive;
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

BGN *BGN_from_integer(intmax_t number) {
    UN temp;
    temp.BGV = malloc(sizeof(BGV));

    Positive positive;
    if (number >= 0) {
        positive = positive_zero;
    } else {
        positive = negative;
    }


    *temp.BGV = (BGV) {
            .capacity = 1,
            .data = malloc(sizeof(uintmax_t)),
            .size = 1,
            .positive = positive
    };
    temp.BGV->data[0] = (uintmax_t) number;
    return temp.BGN;
}

BGN *BGN_from_unsigned(uintmax_t number) {
    UN temp;
    temp.BGV = malloc(sizeof(BGV));

    *temp.BGV = (BGV) {
            .capacity = 1,
            .data = malloc(sizeof(uintmax_t)),
            .size = 1,
            .positive = positive_zero
    };
    temp.BGV->data[0] = number;
    return temp.BGN;
}

uintmax_t BGN_to_unsigned(BGN *number) {
    UN temp;
    temp.BGN = number;

    if (temp.BGV->size == 0) {
        return 0;
    }

    return temp.BGV->data[0];
}

intmax_t BGN_to_integer(BGN *number) {
    UN temp;
    temp.BGN = number;

    if (temp.BGV->size == 0) {
        return 0;
    }

    return ((intmax_t) temp.BGV->data[0]) * (temp.BGV->positive + 1);
}