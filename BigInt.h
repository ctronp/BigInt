// Made by Carlos Troncoso Pizarro
// https://github.com/ctronp/BigInt



#ifndef BIG_INT_BIGINT_H
#define BIG_INT_BIGINT_H

#include <stdint.h>

typedef unsigned char byte;

// BiG Number / default structure to hide implementation
typedef struct {
    uintptr_t size;
} BGN;


// Create New Number
BGN *BGN_new_number();

// Create New Number from an integer
BGN *BGN_from_integer(intptr_t number);

// Create New Number from an unsigned
BGN *BGN_from_unsigned(uintptr_t number);

// Delete Big Number
void BGN_delete(BGN *number);

// Shift Left


#endif //BIG_INT_BIGINT_H
