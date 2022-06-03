// Made by Carlos Troncoso Pizarro



#ifndef BIG_INT_BIGINT_H
#define BIG_INT_BIGINT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned char byte;
// BiG Number
typedef struct {} BGN;

BGN *BGN_new_number();
BGN *BGN_with_capacity(size_t capacity);
void BI_delete(BGN *number);

#endif //BIG_INT_BIGINT_H
