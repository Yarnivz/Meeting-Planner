/*
//============================================================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer, modified by Kasper Engelen
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Declarations for design by contract in C++
//============================================================================

// #include <assert.h>

// #define REQUIRE(assertion, what) \
// 	if (!(assertion)) __assert (what, __FILE__, __LINE__)

// #define ENSURE(assertion, what) \
// 	if (!(assertion)) __assert (what, __FILE__, __LINE__)
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static inline void contract_fail(
    const char* expr,
    const char* file,
    int line,
    const char* fmt, ...)
{
    fprintf(stderr,
            "Contract violation\n"
            "  Expression: %s\n"
            "  Location:   %s:%d\n"
            "  Message:    ",
            expr, file, line);

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);

    va_end(args);
    fputc('\n', stderr);

    abort();
}

#define REQUIRE(assertion, what_fmt, ...) \
    do { \
        if (!(assertion)) \
            contract_fail(#assertion, __FILE__, __LINE__, what_fmt, ##__VA_ARGS__); \
    } while (0)

#define ENSURE(assertion, what_fmt, ...) \
    do { \
        if (!(assertion)) \
            contract_fail(#assertion, __FILE__, __LINE__, what_fmt, ##__VA_ARGS__); \
    } while (0)