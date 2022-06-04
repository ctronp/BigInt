// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt



#pragma once

#include <stdint.h>
#include <stdbool.h>

/// Settings
/// Here you set how the code will run


/// Documentation:

// BiG Number / default structure to hide implementation
typedef struct {
    uintmax_t size;
} BGN;


// Create New Number
BGN *BGN_new_number();

// Create New Number from an integer
BGN *BGN_from_integer(intmax_t number);

// Create New Number from an unsigned
BGN *BGN_from_unsigned(uintmax_t number);

// Delete Big Number
void BGN_delete(BGN *number);

// To uintmax_t (will ignore the sign, and bits over uintmax_t)
uintmax_t BGN_to_unsigned(BGN *number);

// to intmax_t (will ignore bits over uintmax_t)
// will overflow if the number is greater than 2 ^ (sizeof(uintmax_t)*8-1) -1
intmax_t BGN_to_integer(BGN *number);
