// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt



#pragma once

#include <stdint.h>

/// \Documentation
/// Explained using a 64 bits architecture.


/// BiG Number is the default structure, used to hide implementation
typedef struct {
    uintmax_t size;
} BGN;

/// BGN_new_number create a BigNumber initialized in 0
/// \return new Big Number
BGN *BGN_new_number();

/// BGN_from_integer creates a number initialized
/// \param number receive a intmax_t to initialize the Big Number
/// \return new Big Number initialized
BGN *BGN_from_integer(intmax_t number);

/// BGN_from_unsigned creates a number initialized
/// \param number receive a uintmax_t to initialize the Big Number
/// \return new Big Number initialized
BGN *BGN_from_unsigned(uintmax_t number);

/// BGN_delete frees the memory from a number
/// \param number Big Number to delete
void BGN_delete(BGN *number);

/// BGN_to_unsigned convert the last 64 bits to uintmax_t
/// \param number Big Number that will be converted to uintmax_t
/// \return last 64 bits of number as uintmax_t
uintmax_t BGN_to_unsigned(BGN *number);

/// BGN_to_integer convert the last 63 bits to uintmax_t
/// from -(2 ^ 63) to (2 ^ 63).
/// \n doesn't include -(2 ^ 63) -1
/// \param number Big Number that will be converted to intmax_t
/// \return last 64 bits of 'number' as intmax_t
intmax_t BGN_to_integer(BGN *number);

/// BGN_shift_left shift left the number.
/// \param number number to be shifted
/// \param shift number of bits to shift.
/// \return new Big Number, equal to 'number' shifted to left 'shift' times.
BGN *BGN_shift_left(BGN *number, uintmax_t shift);

/// BGN_shift_right shift right the number.
/// \param number number to be shifted
/// \param shift number of bits to shift.
/// \return new Big Number, equal to 'number' shifted to right 'shift' times.
BGN *BGN_shift_right(BGN *number, uintmax_t shift);