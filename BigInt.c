// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt

#include "BigInt.h"

#include <stdlib.h>
#include <string.h>

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

static inline void bg_fit(BGV *vector) {
    while (!vector->data[vector->size]) {
        --vector->size;
    }
    if (vector->size == vector->capacity) {
        vector->capacity = vector->size;
        vector->data = realloc(vector->data, sizeof(uintmax_t) * vector->size);
    }
}

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

static inline BGV *bg_with_capacity_calloc(uintmax_t capacity) {
    UN out;
    out.BGV = malloc(sizeof(BGV));
    *out.BGV = (BGV) {
            .size = 0,
            .capacity = capacity,
            .data = calloc(capacity, sizeof(uintmax_t)),
            .positive = positive_zero
    };
    return out.BGV;
}

static inline void bg_copy_vector(BGV *origin, uintmax_t *destiny) {
    bg_fit(origin);
    for (unsigned i = 0; i < origin->size; i++) {
        destiny[i] = origin->data[i];
    }
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
        out.BGV = bg_with_capacity_calloc(new_zeros + in.BGN->size);
        bg_copy_vector(in.BGV, &out.BGV->data[new_zeros]);
        return out.BGN;
    }

    const uintmax_t diff_bits = (n_bits - shift_size);
    const uintmax_t mask = UINTMAX_MAX << diff_bits;

    out.BGV = bg_with_capacity_calloc(new_zeros + in.BGN->size + 1);
    out.BGV->size = new_zeros + in.BGN->size + 1;

    uintmax_t old_carry = 0, new_carry;
    for (uintmax_t i = new_zeros; i < out.BGV->size; i++) {
        new_carry = (out.BGV->data[i] & mask) >> diff_bits;
        out.BGV->data[i] <<= shift_size;
        out.BGV->data[i] |= old_carry;
        old_carry = new_carry;
    };

    bg_fit(out.BGV);
    return out.BGN;
}