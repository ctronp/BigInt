// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt



#pragma once

#include <stdint.h>
#include <stdbool.h>

/// Enable or disable the facade. Disable by default
#ifndef BGN_HIDE_IMP

/// BiG Vector
typedef struct struct_vector BGN;

#else

/// don't make manual usage from this struct, is just a facade over BGV.
/// \n if you want to manually change the attributes disable BGN_HIDE_IMP
typedef struct {
    const uintmax_t size;
} BGN;

#endif



/// \Documentation
/// Explained using a 64 bits architecture.


/// BiG Number is the default structure, used to hide implementation

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

/// BGN_is_zero returns true if the number is zero
/// \param number number to check if is zero
/// \return true if number is zero
bool BGN_is_zero(BGN *number);

/// cmp with zero
/// \param number number to compare
/// \return positive, zero or negative based on number
int BGN_cmp_zero(BGN *number);


/// BGN_base_10 write in the buffer the base 10 string of the number
/// \param number number to be written
/// \param buffer where the number will be written
void BGN_base_10(BGN *number, char *buffer);


/// check if stdarg.h has been included.
/// \n not using only _STDARG_H, as llvm use __STDARG_H and any compiler can use other one.
#if defined(_STDARG_H) || defined(__STDARG_H) || (defined(va_start) && defined(va_arg) && defined(va_end))
#define STDARG_H

/// delete various numbers with one function
/// \param number first number to delete (free)
/// \param ... other numbers to delete (free)
void BGN_vdelete(BGN *number, ...);

#endif