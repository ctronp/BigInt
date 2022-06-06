//
// Created by carlo on 6/3/2022.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "BigInt.h"

#define IS_INT(x) ((x) * 0 - 1) < (x)
#define INT_PRINT(x) printf(#x": %llu\n", (long long unsigned)(x))
#define UNS_PRINT(x) printf(#x": %lld\n", (long long)(x))
#define DEBUG_P(x) IS_INT(x) ? INT_PRINT(x) : UNS_PRINT(x)

#ifdef _MSC_VER

#include <windows.h>

HANDLE hConsole;
bool w_singleton = false;

static inline void red_terminal() {
    if (!w_singleton) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
}

static inline void green_terminal() {
    if (!w_singleton) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
}

static inline void default_terminal() {
    if (!w_singleton) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    SetConsoleTextAttribute(hConsole, 3);
}

#else

static inline void red_terminal() {
    printf("\033[0;31m");
}

static inline void green_terminal() {
    printf("\033[0;32m");
}

static inline void default_terminal() {
    printf("\033[0;34m");
}

#endif

unsigned test_counter = 0;
bool failure = false;

bool result;
#define test(test_name) \
    do {                \
    result = test_name(); \
    if (!result) {      \
        failure = true;     \
        red_terminal();\
        printf("Test %u: %s\n\tFailed\n\n",test_counter++, #test_name );           \
    } else {              \
        green_terminal();\
        printf("Test %u: %s\n\tPassed\n\n",test_counter++, #test_name);    \
    }                   \
        } while(0)

bool base_number() {
    BGN *t = BGN_new_number();
    BGN_delete(t);
    return true;
}

bool new_to_unsigned() {
    intmax_t rand_value = 1035945505586175809;
    BGN *t1 = BGN_from_integer(rand_value);
    BGN *t2 = BGN_from_unsigned(rand_value);

    bool answer =
            rand_value ==
            BGN_to_unsigned(t1) &&
            BGN_to_unsigned(t1) ==
            BGN_to_integer(t1) &&
            BGN_to_unsigned(t2) ==
            BGN_to_integer(t2) &&
            BGN_to_unsigned(t1) ==
            BGN_to_unsigned(t2);
    BGN_delete(t1);
    BGN_delete(t2);
    return answer;
}

bool new_to_integer() {
    intmax_t rand_value = -1035945505586175809;
    BGN *t1 = BGN_from_integer(rand_value);

    bool answer =
            rand_value ==
            BGN_to_integer(t1);
    BGN_delete(t1);
    return answer;
}

bool shift_left_positive() {
    intmax_t rand_value = 8807508551058554305;
    BGN *t1 = BGN_from_integer(rand_value);
    BGN *t2 = BGN_shift_left(t1, 3);
    bool answer =
            ((rand_value << 3) & (UINTMAX_MAX >> 1)) ==
            BGN_to_integer(t2);
    BGN_delete(t1);
    BGN_delete(t2);
    return answer;
}

bool shift_left_negative() {
    intmax_t rand_value = -8807508551058554305;
    BGN *t1 = BGN_from_integer(rand_value);
    BGN *t2 = BGN_shift_left(t1, 3);
    BGN *t3 = BGN_shift_left(t1, 2);
    BGN *t4 = BGN_shift_left(t3, 1);
    bool answer =
            BGN_to_integer(t2) ==
            BGN_to_integer(t4);
    BGN_delete(t1);
    BGN_delete(t2);
    BGN_delete(t3);
    BGN_delete(t4);
    return answer;
}

bool shift_left_small_negative() {
    intmax_t rand_value = -88574305;
    BGN *t1 = BGN_from_integer(rand_value);
    BGN *t2 = BGN_shift_left(t1, 3);
    bool answer =
            BGN_to_integer(t2) ==
            rand_value * 8;
    BGN_delete(t1);
    BGN_delete(t2);
    return answer;
}

bool shift_right_positive() {
    intmax_t rand_value = 8807508551058554305;
    BGN *t1 = BGN_from_integer(rand_value);
    BGN *t2 = BGN_shift_right(t1, 3);
    bool answer =
            ((rand_value >> 3) & (UINTMAX_MAX >> 1)) ==
            BGN_to_integer(t2);
    BGN_delete(t1);
    BGN_delete(t2);
    return answer;
}

int main() {
    default_terminal();

    printf("bits in uintmax_t: %lu\n\n\n", sizeof(uintmax_t) * 8);
    test(base_number);
    test(new_to_unsigned);
    test(new_to_integer);
    test(shift_left_positive);
    test(shift_left_negative);
    test(shift_left_small_negative);
    test(shift_right_positive);

    if (failure) exit(1);
    exit(0);
}






