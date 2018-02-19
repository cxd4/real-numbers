#include <errno.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include <float.h>
#include <math.h>

#include "strtor.h"

static long double
my_sqrt(long double x)
{
    long double newp, oldp;

    for (oldp = x, newp = x/2; newp != oldp; newp = (oldp + x/oldp)/2) {
        oldp = newp;
    }
    return (newp);
}

real pi(void)
{
    static real pi_final;
    long double pi_approximation, old_approximation, base;

    if (pi_final != 0)
        return (pi_final);
    base = 0;
    pi_approximation = 2;
    do {
        old_approximation = pi_approximation;
        base = my_sqrt(base + 2);
        pi_approximation *= 2;
        pi_approximation /= base;
    } while (pi_approximation != old_approximation); /* no more precision */
    pi_final = (real)(pi_approximation);
    return (pi_final);
}

real
strtor(const char* str)
{
    register real result;
    char* new_offset;

    if (errno == ERANGE) {
        fprintf(stderr, "Unchecked errno already set to %i.\n", errno);
        errno = 0;
    }
    if (str[0] == '\0') {
        result = 0;
    } else if (str[0] == 'e') {
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
    } else if (strncmp(str, "sin", 3) == 0) {
        result = sin(strtor(str + 3) / 180 * pi());
    } else if (strncmp(str, "cos", 3) == 0) {
        result = cos(strtor(str + 3) / 180 * pi());
    } else if (strncmp(str, "tan", 3) == 0) {
        result = tan(strtor(str + 3) / 180 * pi());
    } else if (strncmp(str, "csc", 3) == 0) {
        result = 1 / sin(strtor(str + 3) / 180 * pi());
    } else if (strncmp(str, "sec", 3) == 0) {
        result = 1 / cos(strtor(str + 3) / 180 * pi());
    } else if (strncmp(str, "cot", 3) == 0) {
        result = 1 / tan(strtor(str + 3) / 180 * pi());
    } else {
        result = strtod(str, &new_offset);
        if (new_offset != str) {
            switch (*new_offset) {
            case '\0':
                break;
            case '/': /* fraction:  1/2e might mean 1/2 of e or 1/(2e). */
            case ':': /* ratio:  1:2e might mean 1:(2e) or 1/2 of e. */
                ++(new_offset);
                if (*new_offset != '\0')
                    result /= strtor(new_offset);
                break;
            case '^':
                result = pow(result, strtor(new_offset + 1));
                break;
            default:
                result *= strtor(new_offset);
            }
        }
    }
    if (errno == ERANGE) {
        fprintf(
            stderr, "Problem %i converting %s to %Lf.\n",
            errno, str, (long double)result
        );
        errno = 0;
    }
    return (result);
}

integer
strtoi(const char* str)
{
    register integer result;

    if (errno == ERANGE) {
        fprintf(stderr, "Unchecked errno already set to %i.\n", errno);
        errno = 0;
    }
    result = strtol(str, NULL, 0);
    if (errno == ERANGE) {
        fprintf(stderr, "Problem %i converting %s.\n", errno, str);
        errno = 0;
    }
    return (result);
}

whole_number
strtow(const char* str)
{
    register whole_number result;

    if (errno == ERANGE) {
        fprintf(stderr, "Unchecked errno already set to %i.\n", errno);
        errno = 0;
    }
    result = strtoul(str, NULL, 0);
    if (errno == ERANGE) {
        fprintf(stderr, "Problem %i converting %s.\n", errno, str);
        errno = 0;
    }
    return (result);
}

int
puterr(const char* message)
{
    return fprintf(stderr, "%s\n", message);
}

int
fp_str_round_trip(void)
{
    static int fp_digits;
#if ((FLT_RADIX) % 2 == 0)
    real test_ratio; /* (real) can be either (float) or ([long] double). */
    int lsd_of_whole_part; /* (int)66.666666667 % 10 == 6 */
    const int correct_lsd = 3; /* We don't want a .66... case since 7 counts. */
#endif

    if (fp_digits)
        return (fp_digits);
#if ((FLT_RADIX) % 1 != 0)
#ifdef DBL_DECIMAL_DIG
    fp_digits = DBL_DECIMAL_DIG - 1; /* defined in 2011 C specifications */
#elif defined(DECIMAL_DIG)
    fp_digits =
        (sizeof(real) == sizeof(long double))
      ? DECIMAL_DIG - 1 /* defined in C99 */
      : DBL_DIG + 2
    ;
#else
    fprintf(stderr,
        "Untested:  Derive round precision when FLT_RADIX = %i?\n", FLT_RADIX
    ); /* If FLT_RADIX is 9, the (1/9) rounding algorithm below is useless. */
    fp_digits = DBL_DIG;
#endif
#else
    test_ratio = (real)(correct_lsd) / (10 - 1); /* 0.111..., 0.222..., etc. */
    test_ratio *= 10;
    lsd_of_whole_part = correct_lsd; /* 1.11..., 2.22..., etc. */
    for (fp_digits = 1; lsd_of_whole_part == correct_lsd; fp_digits++) {
        test_ratio *= 10;
        test_ratio -= 10 * correct_lsd; /* Prevent overflow when casting. */
        lsd_of_whole_part = (int)((unsigned int)test_ratio % 10);
    }
    --fp_digits;
#endif
    if (fp_digits < FLT_DIG) /* This should never happen. */
        fp_digits = FLT_DIG;
    return (fp_digits);
}
