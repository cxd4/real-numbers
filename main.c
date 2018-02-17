#include <stdio.h>
#include <string.h>

#include <signal.h>
#include <setjmp.h>

static jmp_buf CPU_state;
static void FPU_exception_handler(int signal_code)
{
    fprintf(stderr, "Caught ALU exception with signal %i.\n", signal_code);
    longjmp(CPU_state, 1);
} /* overflow, division by zero, etc. */

#include "calc.h"
#include "strtor.h"

static const char ops[][8] = {
    "add",
    "sub",
    "mul",
    "div",

/*
 * Because a partial search on "mod" (the % operator in C) matches "mode",
 * we want "mode" to be defined before "mod" in the strncmp() search order.
 */
    "mean",
    "med",
    "mode",
    "range",

    "mod",
    "abs",
    "ceil",
    "floor",

    "pow",
    "bexp",
    "root",
    "bradix",

    "fact",
    "comp",
    "gcd",
    "lcm",

    "arcsin",
    "arccos",
    "arctan",
};

static int
f_execute(int argc, char* argv[])
{
    register size_t i, j;
    const size_t limit = sizeof(ops) / sizeof(ops[0]);

    for (i = 0; i < limit; i++) {
        const char* name_start;
        int recovered_from_exception;
        int error_code;

        name_start = argv[0];
        switch (*name_start) {
        case 'i':
            j = 1;
            ++name_start;
            break;
        case 'w':
            j = 2;
            ++name_start;
            break;
        default:
            j = 0;
        }
        if (strncmp(name_start, ops[i], strlen(ops[i])) != 0)
            continue;
        signal(SIGFPE, FPU_exception_handler);
        recovered_from_exception = setjmp(CPU_state);
        if (recovered_from_exception) {
            signal(SIGFPE, FPU_exception_handler);
            continue; /* Try searching for another function to execute. */
        }
        error_code = op_functions[3*i + j](argc, &argv[0]);
        if (error_code != 0)
            return (error_code);
        switch (argv[0][0]) {
        case 'i':
            fprintf(stdout, "%li\n", i_result);
            break;
        case 'w':
            fprintf(stdout, "%lu\n", w_result);
            break;
        default:
            fprintf(stdout,
                "%.*Lg\n", fp_str_round_trip(), (long double)r_result
            );
        }
        return (error_code);
    }
    return 1;
}

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "%s [operands]\n", argv[0]);
        return -4;
    }
    switch (f_execute(argc - 1, &argv[1])) {
    case -1:
        puterr("Insufficient operands.");
        return -1;
    case  0:
        return 0;
    case +1:
        fprintf(stderr, "Cannot execute operation \"%s\".\n", argv[1]);
        return 1;
    case +2:
        puterr("Requested null or undefined operation.");
        return 2;
    default:
        break;
    }
    puterr("Unknown error return status.");
    return -2;
}

/*
 * provided to reduce EXE file size of the MS-DOS build by DJGPP
 *
 * DJGPP      :  gcc -o rc_debug.exe *.c -Os -Wall -pedantic -lc -lm
 * Strip with :  strip -o rc.exe rc_debug.exe -s
 *
 * Open Watcom:  wcl386 -fe=rc *.c -d0 -os -s -bcl=stub32xc
 */
#ifdef __DJGPP__
#include <crt0.h>

char**
__crt0_glob_function(char* arg)
{
    return NULL;
}
void
__crt0_load_environment_file(char* progname)
{
    return;
}
#endif
