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


void bg_fit(BGN *vector) {
    while (vector->size && !vector->data[vector->size - 1]) {
        --vector->size;
    }
    if (vector->size != vector->capacity) {
        vector->capacity = vector->size;
        vector->data = realloc(vector->data, sizeof(uintmax_t) * vector->size);
    }
}

void bg_resize(BGN *vector, uintmax_t size) {
    if (!vector->size) {
        vector->data = malloc(sizeof(uintmax_t) * size);
        vector->size = size;
        return;
    }

    vector->data = realloc(vector->data, size);
    if (size > vector->size)
        // TODO replace with memset
        for (unsigned i = vector->size; i < size; i++) {
            vector->data[i] = 0;
        }
    vector->size = size;
}

void bg_append(BGN *vector, uintmax_t value) {
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

size_t bg_byte_capacity(BGN *vector) {
    return vector->capacity * sizeof(uintmax_t);
}

size_t bg_byte_size(BGN *vector) {
    return vector->size * sizeof(uintmax_t);
}

BGN *bg_with_capacity_calloc(uintmax_t capacity) {
    BGN *out;
    out = malloc(sizeof(BGN));
    *out = (BGN) {
            .size = 0,
            .capacity = capacity,
            .data = calloc(capacity, sizeof(uintmax_t)),
            .positive = positive_zero
    };
    return out;
}

void bg_copy_vector(BGN *origin, uintmax_t *destiny) {
    bg_fit(origin);
    for (unsigned i = 0; i < origin->size; i++) {
        destiny[i] = origin->data[i];
    }
}

void bg_copy_to_vector(const uintmax_t size, const uintmax_t *origin, BGN *destiny) {
    for (uintmax_t i = 0; i < size; i++) {
        destiny->data[i] = origin[i];
    }
}

BGN *BGN_new_number() {
    return (BGN *) calloc(1, sizeof(BGN));
}


void BGN_delete(BGN *number) {
    free(number->data);
    free(number);
}

void BGN_vdelete(BGN *number, ...) {
    va_list ptr;
    // TODO finish this function
}

BGN *BGN_from_integer(intmax_t number) {
    BGN *out;
    out = malloc(sizeof(BGN));

    Positive positive;
    if (number >= 0) {
        positive = positive_zero;
    } else {
        positive = negative;
        number *= -1;
    }


    *out = (BGN) {
            .capacity = 1,
            .data = malloc(sizeof(uintmax_t)),
            .size = 1,
            .positive = positive
    };
    out->data[0] = (uintmax_t) number;
    return out;
}

BGN *BGN_from_unsigned(uintmax_t number) {
    BGN *out;
    out = malloc(sizeof(BGN));

    *out = (BGN) {
            .capacity = 1,
            .data = malloc(sizeof(uintmax_t)),
            .size = 1,
            .positive = positive_zero
    };
    out->data[0] = number;
    return out;
}

uintmax_t BGN_to_unsigned(BGN *number) {
    if (number->size == 0) {
        return 0;
    }

    return number->data[0];
}

intmax_t BGN_to_integer(BGN *number) {
    if (number->size == 0) {
        return 0;
    }

    if (number->positive == negative) {
        return -((intmax_t) (number->data[0] & (UINTMAX_MAX >> 1)));
    }
    return (intmax_t) (number->data[0] & (UINTMAX_MAX >> 1));
}

BGN *BGN_shift_left(BGN *number, uintmax_t shift) {
    BGN *out;

    const unsigned new_zeros = shift / n_bits;
    const unsigned shift_size = shift % n_bits;

    if (shift_size == 0) {
        out = bg_with_capacity_calloc(new_zeros + number->size);
        out->size = new_zeros + number->size;
        out->positive = number->positive;
        bg_copy_vector(number, &out->data[new_zeros]);
        return out;
    }

    const uintmax_t diff_bits = (n_bits - shift_size);

    out = bg_with_capacity_calloc(new_zeros + number->size + 1);
    out->size = new_zeros + number->size + 1;
    out->positive = number->positive;

    uintmax_t carry = 0;
    for (uintmax_t i = 0; i < number->size; i++) {
        const uintmax_t j = i + new_zeros;
        out->data[j] = number->data[i] << shift_size;
        out->data[j] |= carry;
        carry = number->data[i] >> diff_bits;
    }

    bg_fit(out);
    return out;
}

BGN *BGN_shift_right(BGN *number, uintmax_t shift) {
    BGN *out;

    const unsigned less_size = shift / n_bits;
    const unsigned shift_size = shift % n_bits;

    if (shift_size == 0) {
        out = bg_with_capacity_calloc(number->size - less_size);
        out->size = number->size - less_size;
        out->positive = number->positive;
        bg_copy_to_vector(number->size - less_size, &number->data[less_size], out);
        return out;
    }

    const uintmax_t diff_bits = (n_bits - shift_size);

    out = bg_with_capacity_calloc(number->size - less_size);
    out->size = number->size - less_size;
    out->positive = number->positive;

    uintmax_t carry = 0;
    for (uintmax_t i = 0; i < out->size; i++) {
        const uintmax_t in_pos = number->size - 1 - i;
        const uintmax_t out_pos = out->size - 1 - i;
        out->data[out_pos] = number->data[in_pos] >> shift_size;
        out->data[out_pos] |= carry;
        carry = number->data[in_pos] << diff_bits;
    }

    bg_fit(out);
    return out;
}

bool BGN_is_zero(BGN *number) {
    bg_fit(number);
    return (number->size != 0);
}

int BGN_cmp_zero(BGN *number) {
    bg_fit(number);

    if (number->size) {
        if (number->positive == positive_zero) return 1;
        return -1;
    }
    return 0;
}

// TODO use grade-school multiplication
BGN *BGN_multiply(BGN *in1, BGN *in2) {
    BGN *out;

    bg_fit(in1);
    bg_fit(in2);

    out = bg_with_capacity_calloc(2 * (in1->size + in2->size));
    out->size = 2 * (in1->size + in2->size);

    while (in1->size < in2->size);
    return out;
}


// TODO complete this function
void BGN_base_10(BGN *number, char *buffer) {
    \
// Recommended lecture: https://johnnylee-sde.github.io/Fast-unsigned-integer-to-string/

}