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
    while (vector->size && !vector->data[vector->size - 1]) {
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

static inline void bg_copy_to_vector(const uintmax_t size, const uintmax_t *origin, BGV *destiny) {
    for (uintmax_t i = 0; i < size; i++) {
        destiny->data[i] = origin[i];
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

BGN *BGN_shift_left(BGN *number, uintmax_t shift) {
    UN in;
    UN out;

    in.BGN = number;
    const unsigned new_zeros = shift / n_bits;
    const unsigned shift_size = shift % n_bits;

    if (shift_size == 0) {
        out.BGV = bg_with_capacity_calloc(new_zeros + in.BGN->size);
        out.BGV->size = new_zeros + in.BGN->size;
        out.BGV->positive = in.BGV->positive;
        bg_copy_vector(in.BGV, &out.BGV->data[new_zeros]);
        return out.BGN;
    }

    const uintmax_t diff_bits = (n_bits - shift_size);

    out.BGV = bg_with_capacity_calloc(new_zeros + in.BGN->size + 1);
    out.BGV->size = new_zeros + in.BGN->size + 1;
    out.BGV->positive = in.BGV->positive;

    uintmax_t carry = 0;
    for (uintmax_t i = 0; i < in.BGV->size; i++) {
        const uintmax_t j = i + new_zeros;
        out.BGV->data[j] = in.BGV->data[i] << shift_size;
        out.BGV->data[j] |= carry;
        carry = in.BGV->data[i] >> diff_bits;
    }

    bg_fit(out.BGV);
    return out.BGN;
}

// TODO solve bug in test 6
BGN *BGN_shift_right(BGN *number, uintmax_t shift) {
    UN in;
    UN out;

    in.BGN = number;
    const unsigned less_size = shift / n_bits;
    const unsigned shift_size = shift % n_bits;

    if (shift_size == 0) {
        out.BGV = bg_with_capacity_calloc(in.BGN->size - less_size);
        out.BGV->size = in.BGN->size - less_size;
        out.BGV->positive = in.BGV->positive;
        bg_copy_to_vector(in.BGV->size - less_size, &in.BGV->data[less_size], out.BGV);
        return out.BGN;
    }

    const uintmax_t diff_bits = (n_bits - shift_size);

    out.BGV = bg_with_capacity_calloc(in.BGN->size - less_size);
    out.BGV->size = in.BGN->size - less_size;
    out.BGV->positive = in.BGV->positive;

    uintmax_t carry = 0;
    for (uintmax_t i = 0; i < in.BGV->size; i++) {
        const uintmax_t in_pos = in.BGV->size - 1 - i;
        const uintmax_t out_pos = out.BGV->size - 1 - i;
        out.BGV->data[out_pos] = in.BGV->data[in_pos] >> shift_size;
        out.BGV->data[out_pos] |= carry;
        carry = in.BGV->data[in_pos] << diff_bits;
    }

    bg_fit(out.BGV);
    return out.BGN;
}