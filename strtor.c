#include <errno.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include <math.h>
#include "strtor.h"

real
strtor(const char* str)
{
    register double result;

    if (errno) {
        fprintf(stderr, "Unchecked errno already set to %i.\n", errno);
        errno = 0;
    }
    if (str[0] == 'e') {
        if (str[1] == '\0')
            result = exp(1);
        else
            result = exp(strtor(str + 1));
    } else {
        result = strtod(str, NULL);
    }
    if (errno) {
        fprintf(
            stderr, "Problem %i converting %s to %f.\n",
            errno, str, result
        );
    }
    return (result);
}

integer
strtoi(const char* str)
{
    register long result;

    if (errno) {
        fprintf(stderr, "Unchecked errno already set to %i.\n", errno);
        errno = 0;
    }
    result = strtol(str, NULL, 0);
    if (errno) {
        fprintf(
            stderr, "Problem %i converting %s to %li.\n",
            errno, str, result
        );
    }
    return (result);
}

whole_number
strtow(const char* str)
{
    register unsigned long result;

    if (errno) {
        fprintf(stderr, "Unchecked errno already set to %i.\n", errno);
        errno = 0;
    }
    result = strtoul(str, NULL, 0);
    if (errno) {
        fprintf(
            stderr, "Problem %i converting %s to %lu.\n",
            errno, str, result
        );
    }
    return (result);
}
