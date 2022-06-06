//
// Created by carlo on 6/3/2022.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "BigInt.h"

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
    intmax_t rand_value = 5807505;
    BGN *t1 = BGN_from_integer(rand_value);
    BGN *t2 = BGN_shift_left(t1, 3);
    printf("%lu", BGN_to_unsigned(t2));
    bool answer =
            (rand_value << 3) ==
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

    if (failure) exit(1);
    exit(0);
}






