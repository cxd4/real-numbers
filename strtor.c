#include <errno.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include <math.h>
#include "strtor.h"

static real pi(void)
{
    static real pi_approximation;

    if (pi_approximation != 0)
        return (pi_approximation);
    pi_approximation = 3.14159265358979323846264338327950288419;
    return (pi_approximation);
}

real
strtor(const char* str)
{
    register double result;
    char* new_offset;

    if (errno) {
        fprintf(stderr, "Unchecked errno already set to %i.\n", errno);
        errno = 0;
    }
    if (str[0] == 'e') {
        if (str[1] == '\0')
            result = exp(1);
        else
            result = exp(strtor(str + 1));
    } else if (str[0] == 'p' && str[1] == 'i') {
        if (str[2] == '\0')
            result = pi();
        else
            result = pow(pi(), strtor(str + 2));
    } else if (strncmp(str, "sqrt", 4) == 0) {
        result = sqrt(strtor(str + 4));
    } else if (strncmp(str, "cbrt", 4) == 0) {
        result = strtor(str + 4);
        result = (result < 0) ? -pow(-result, 1.0 / 3) : pow(result, 1.0 / 3);
    } else {
        result = strtod(str, &new_offset);
        if (*new_offset != '\0')
            result *= strtor(new_offset);
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
