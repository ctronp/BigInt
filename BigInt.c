// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt

#include "BigInt.h"

#include <stdlib.h>

typedef unsigned char byte;

const uintmax_t n_bits = sizeof(uintmax_t) * 8;

/// Positive Number enum
typedef enum struct_positive {
    positive_zero = 0,
    negative = 1,
} Positive;

/// BiG Vector
typedef struct struct_vector {
    uintmax_t size;
    uintmax_t capacity;
    uintmax_t *data;
    uint8_t positive;
} BGV;

/// Union to change file type
typedef union {
    BGN *BGN;
    BGV *BGV;
} UN;

static inline void bg_append(BGV *vector, uintmax_t value) {
    if (vector->capacity != vector->size) {
        vector->data[vector->size++] = value;
    } else if (vector->capacity == 0) {
        vector->capacity = 1;
        vector->data = malloc(sizeof(uintmax_t));
        vector->data[0] = value;
    } else {
        vector->capacity <<= 2;
        vector->data = realloc(vector->data, vector->capacity);
        vector->data[vector->size++] = value;
    }
}

static inline size_t bg_byte_capacity(BGV *vector) {
    return vector->capacity * sizeof(uintmax_t);
}

static inline size_t bg_byte_size(BGV *vector) {
    return vector->size * sizeof(uintmax_t);
}

static inline BGV *bg_with_capacity(uintmax_t capacity) {
    UN out;
    out.BGV = malloc(sizeof(BGV));
    *out.BGV = (BGV) {
            .size = 0,
            .capacity = capacity,
            .data = malloc(sizeof(uintmax_t) * capacity),
            .positive = positive_zero
    };
    return out.BGV;
}

BGN *BGN_new_number() {
    return (BGN *) calloc(1, sizeof(BGV));
}


void BGN_delete(BGN *number) {
    UN in;
    in.BGN = number;
    free(in.BGV->data);
    free(number);
}

BGN *BGN_from_integer(intmax_t number) {
    UN out;
    out.BGV = malloc(sizeof(BGV));

    Positive positive;
    if (number >= 0) {
        positive = positive_zero;
    } else {
        positive = negative;
        number *= -1;
    }


    *out.BGV = (BGV) {
            .capacity = 1,
            .data = malloc(sizeof(uintmax_t)),
            .size = 1,
            .positive = positive
    };
    out.BGV->data[0] = (uintmax_t) number;
    return out.BGN;
}

BGN *BGN_from_unsigned(uintmax_t number) {
    UN out;
    out.BGV = malloc(sizeof(BGV));

    *out.BGV = (BGV) {
            .capacity = 1,
            .data = malloc(sizeof(uintmax_t)),
            .size = 1,
            .positive = positive_zero
    };
    out.BGV->data[0] = number;
    return out.BGN;
}

uintmax_t BGN_to_unsigned(BGN *number) {
    UN in;
    in.BGN = number;

    if (in.BGV->size == 0) {
        return 0;
    }

    return in.BGV->data[0];
}

intmax_t BGN_to_integer(BGN *number) {
    UN in;
    in.BGN = number;

    if (in.BGV->size == 0) {
        return 0;
    }

    if (in.BGV->positive == negative) {
        return -((intmax_t) (in.BGV->data[0] & (UINTMAX_MAX >> 1)));
    }
    return (intmax_t) (in.BGV->data[0] & (UINTMAX_MAX >> 1));
}

//TODO end this function
BGN *BGN_shift_left(BGN *number, uintmax_t shift) {
    UN in;
    UN out;

    in.BGN = number;
    unsigned new_zeros = shift / n_bits;
    unsigned shift_size = shift % n_bits;

    if (shift_size == 0) {
        out.BGV = bg_with_capacity(new_zeros + in.BGN->size);
        //TODO copy uintmax_t from in to out.
    }

    out.BGV = bg_with_capacity(new_zeros + in.BGN->size + 1);

    uintmax_t carry;
    uintmax_t mask = UINTMAX_MAX << (n_bits - shift_size);
    for (uintmax_t i = new_zeros; i < out.BGV->capacity; i++) {

    };


    while (out.BGV->data[out.BGV->size] == 0) --out.BGV->size;
    return out.BGN;
}