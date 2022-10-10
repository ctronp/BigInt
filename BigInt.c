// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt

#include "BigInt.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//typedef unsigned char byte;

const uintmax_t n_bits = sizeof(uintmax_t) * 8;
const uintmax_t h_bits = n_bits / 2;
const uintmax_t left_mask = UINTMAX_MAX << h_bits;
const uintmax_t right_mask = UINTMAX_MAX >> h_bits;

#ifdef UINT128_MAX

typedef uint64_t uinthalf_t;
typedef int64_t inthalf_t;

#elif UINT64_MAX

typedef uint32_t uinthalf_t;
typedef int32_t inthalf_t;

#elif UINT32_MAX

typedef uint16_t uinthalf_t;
typedef int16_t inthalf_t;

#elif UINT16_MAX

typedef uint8_t uinthalf_t;
typedef int8_t inthalf_t;

#else

typedef uint8_t uinthalf_t;
typedef uint8_t inthalf_t;

#endif

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

void bg_fit(BGV *vector) {
    while (vector->size && !vector->data[vector->size - 1]) {
        --vector->size;
    }
    if (vector->size != vector->capacity) {
        vector->capacity = vector->size;
        vector->data = realloc(vector->data, sizeof(uintmax_t) * vector->size);
    }
}

void bg_resize(BGV *vector, uintmax_t size) {
    if (!vector->size) {
        vector->data = malloc(sizeof(uintmax_t) * size);
        vector->size = size;
        return;
    }

    realloc(vector->data, size);
    if (size > vector->size)
        for (unsigned i = vector->size; i < size; i++) {
            vector->data[i] = 0;
        }
    vector->size = size;
}

void bg_append(BGV *vector, uintmax_t value) {
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

size_t bg_byte_capacity(BGV *vector) {
    return vector->capacity * sizeof(uintmax_t);
}

size_t bg_byte_size(BGV *vector) {
    return vector->size * sizeof(uintmax_t);
}

BGV *bg_with_capacity_calloc(uintmax_t capacity) {
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

void bg_copy_vector(BGV *origin, uintmax_t *destiny) {
    bg_fit(origin);
    for (unsigned i = 0; i < origin->size; i++) {
        destiny[i] = origin->data[i];
    }
}

void bg_copy_to_vector(const uintmax_t size, const uintmax_t *origin, BGV *destiny) {
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

void BGN_vdelete(BGN *number, ...) {
    va_list ptr;

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
    for (uintmax_t i = 0; i < out.BGV->size; i++) {
        const uintmax_t in_pos = in.BGV->size - 1 - i;
        const uintmax_t out_pos = out.BGV->size - 1 - i;
        out.BGV->data[out_pos] = in.BGV->data[in_pos] >> shift_size;
        out.BGV->data[out_pos] |= carry;
        carry = in.BGV->data[in_pos] << diff_bits;
    }

    bg_fit(out.BGV);
    return out.BGN;
}

bool BGN_is_zero(BGN *number) {
    UN in;
    in.BGN = number;

    bg_fit(in.BGV);

    return (in.BGV->size != 0);
}

int BGN_cmp_zero(BGN *number) {
    UN in;
    in.BGN = number;

    bg_fit(in.BGV);

    if (in.BGV->size) {
        if (in.BGV->positive == positive_zero) return 1;
        return -1;
    }
    return 0;
}

// Based on Karatsuba Algorithm
BGN *BGN_multiply(BGN *number1, BGN *number2) {
    UN in1, in2, out;
    in1.BGN = number1;
    in2.BGN = number2;
    out.BGV = bg_with_capacity_calloc(2 * (in1.BGV->size + in2.BGV->size));
    out.BGV->size = 2 * (in1.BGV->size + in2.BGV->size);

    while (in1.BGV->size < in2.BGV->size);
    return out.BGN;
}


// TODO complete this function
void BGN_base_10(BGN *number, char *buffer) {
    UN in;
    in.BGN = number;
// Recommended lecture: https://johnnylee-sde.github.io/Fast-unsigned-integer-to-string/

}