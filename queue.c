#include <stdlib.h>
#include <stdio.h>

#include <ctype.h>
#include <limits.h>
#include <string.h>

#include "queue.h"
#include "strtor.h"

int
op_queue(void)
{
    static char* user_input;
    static const char* not_enough_RAM =
        "Insufficient memory to upload user query.";

    for (;;) { /* Infinite loop.  Use "exit" operation to escape. */
        char** optv;
        int optc, opti;
        int in_quotes;
        int status_code;
        register size_t i, j;

        optc = 0;
        in_quotes = 0;
        for (i = 0; i < UINT_MAX; i++) {
            int character;

            user_input = realloc(user_input, i + 1);
            if (user_input == NULL) {
                puterr(not_enough_RAM);
                return 1;
            }
            user_input[i] = '\0';
            character = fgetc(stdin);
            if (character == '\n') {
                optc += in_quotes; /* Just add the end quote in for the user. */
                if (!in_quotes && user_input[i - 1] != '\0')
                    ++optc;
                break;
            }

            if (character == EOF || feof(stdin) || ferror(stdin)) {
                puterr("Error while uploading user input.");
                return 1;
            }
            if (character < 0 || character > CHAR_MAX) {
                puterr("Range error while converting input.");
                return 1;
            }

            if (in_quotes == 0 && isspace(character)) {
                character = '\0';
                if (i > 0)
                    optc += (user_input[i - 1] != '\0') ? 1 : 0;
            } else if (character == '"') {
                ++in_quotes;
                if (in_quotes >= 2) {
                    optc += (1);
                    in_quotes = 0;
                }
                character = '\0';
            }
            user_input[i] = (char)(character);
        }

        if (optc == 0)
            continue;
        optv = (char**)calloc((size_t)optc, sizeof(void*));
        if (optv == NULL) {
            puterr(not_enough_RAM);
            break;
        }
        for (j = 0, opti = 0; opti < optc; opti++) {
            size_t arg_length;

            while (user_input[j] == '\0')
                j += 1;
            arg_length = strlen(user_input + j) + sizeof("");

            optv[opti] = (char*)malloc(arg_length);
            if (optv[opti] == NULL) {
                puterr(not_enough_RAM);
                return 1;
            }
            strcpy(optv[opti], user_input + j);
            j += arg_length;
        }
        free(user_input);
        user_input = NULL; /* needed so realloc() behaves like malloc() */

        status_code = f_execute(optc, optv);
        while (optc > 0)
            free(optv[--optc]);
        free(optv);

        switch (status_code) {
        case 0:
            continue;
        case 0x7FFF: /* User executed "exit" operation with no operands. */
            return 0;

        case -1:
            puterr("Insufficient operands.");
            break;
        case +1:
            puterr("Requested operation cannot be executed.");
            break;
        case -2:
            puterr("Unknown operation.");
            break;
        case +2:
            puterr("Requested null or undefined operation.");
            break;
        default:
            fprintf(stderr, "Unknown operation error:  %i.\n", status_code);
            return (status_code);
        }
    }
    free(user_input);
    return 1;
}

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

    "mod", /* "mod[ulus]", "mod[ulo]", whatever, just not "mode"! */
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

    "exit" /* Just exit the command queue loop (if there is one). */
};

int
f_execute(int argc, char* argv[])
{
    register size_t i, j;
    const size_t limit = sizeof(ops) / sizeof(ops[0]);

    for (i = 0; i < limit; i++) {
        const char* name_start;
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
        error_code = op_functions[3*i + j](argc, &argv[0]);
        if (error_code != 0)
            return (error_code);
        switch (argv[0][0]) {
        case 'i':
            fprintf(stdout,
#ifdef _WIN64
                "%lli\n", i_result
#else
                "%li\n", i_result
#endif
            );
            break;
        case 'w':
            fprintf(stdout,
#ifdef _WIN64
                "%llu\n", w_result
#else
                "%lu\n", w_result
#endif
            );
            break;
        default:
            fprintf(stdout,
                "%.*Lg\n", fp_str_round_trip(), (long double)r_result
            );
        }
        return (error_code);
    }
    return -2;
}
