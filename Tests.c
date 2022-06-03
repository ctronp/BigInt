//
// Created by carlo on 6/3/2022.
//

#include <stdbool.h>
#include <stdio.h>
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

static inline void black_terminal() {
    if (!w_singleton) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    SetConsoleTextAttribute(hConsole, 0);
}

#else

static inline void red_terminal() {
    printf("\033[0;31m")
}

static inline void green_terminal() {
    printf("\033[0;32m")
}

static inline void black_terminal() {
    printf("\033[0;30m")
}

#endif

unsigned test_counter = 0;

bool result;
#define test(test_name) \
    result = test_name(); \
    if (!result) {                      \
        red_terminal();\
        printf("Test %u: %s\n\tFailed\n\n",test_counter, #test_name );           \
    } else {              \
        green_terminal();\
        printf("Test %u: %s\n\tPassed\n\n",test_counter, #test_name);    \
    }

bool base_number() {
    BGN *t = BGN_new_number();
    BGN_delete(t);
    return true;
}

int main() {
    test(base_number)
}






