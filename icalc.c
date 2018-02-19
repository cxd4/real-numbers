#include <float.h>
#include <signal.h>

#include "calc.h"
#include "strtor.h"

/* labs(), ldiv(), malloc(), free(), calloc(), qsort(), exit() */
#include <stdlib.h>

#include <math.h>
#include <limits.h>
integer i_result;

static integer ftoi_round(real constant)
{
    real answer, fractional_part;
    double whole_part; /* unused but needs to exist for modf() safety */

    fractional_part = modf(constant, &whole_part);
#if 0
    assert(constant == whole_part + fractional_part);
    assert(fractional_part >= -1 && fractional_part <= +1);
#endif
    if (fractional_part < 0)
        answer = (fractional_part > -.5) ? ceil(constant) : floor(constant);
    else
        answer = (fractional_part < +.5) ? floor(constant) : ceil(constant);
    return (integer)(answer);
}

int iadd(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtoi(argv[i]);
        answer += source;
    }
    i_result = answer;
    return 0;
}
int isubtract(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtoi(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtoi(argv[i]);
        answer -= source;
    }
    i_result = answer;
    return 0;
}
int imultiply(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtoi(argv[i]);
        answer *= source;
    }
    i_result = answer;
    return 0;
}
int idivide(int argc, char* argv[])
{
    ldiv_t quotient;
    integer answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtoi(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtoi(argv[i]);
#ifndef SIGFPE
        if (source == 0) {
            answer = (answer < 0) ? LONG_MIN : LONG_MAX;
            continue;
        } /* Geometric graphs of (n / x) draw best from this approximation. */
#endif
        quotient = ldiv(answer, source);
        answer = quotient.quot;
    }
    i_result = answer;
    return 0;
}
int imodulo(int argc, char* argv[])
{
    ldiv_t quotient;
    integer answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtoi(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtoi(argv[i]);
#ifndef SIGFPE
        if (source == 0)
            return 1;
#endif
        quotient = ldiv(answer, source);
        answer = quotient.rem;
    }
    i_result = answer;
    return 0;
}
int iabsval(int argc, char* argv[])
{
    integer source;

    if (argc < 2)
        return -1;
    source = strtoi(argv[1]);
#ifndef SIGFPE
    if (source == LONG_MIN)
        return 1;
#endif
    i_result = labs(source);
    return 0;
}
int ipower(int argc, char* argv[])
{
    ldiv_t quotient;
    integer answer;
    integer base, power;
    register int i;

    if (argc < 2)
        return -1;
    base = strtoi(argv[1]);
    power = 1;
    for (i = 2; i < argc; i++)
        power *= strtoi(argv[i]);

    if (power < 0) {
        quotient.quot = 0; /* (b ^ -x) = 1/(b ^ x):  truncated to (int) = 0 */
        if (labs(base) == 1 && power == -1) /* ...unless base = +/- 1 */
            quotient.quot = base; /* 1 / -1 == -1; 1 / +1 == +1 */
#ifdef SIGFPE
        else if (base == 0)
            quotient = ldiv(1L, base); /* one divided by zero */
#endif
        i_result = quotient.quot;
    } else {
        answer = +1;
        while (power-- > 0)
            answer *= base;
        i_result = answer;
    }
    return 0;
}
int ibexp(int argc, char* argv[])
{
    integer significand, source;
    int power;

    if (argc < 3)
        return -1;
    significand = strtoi(argv[1]);
    source      = strtoi(argv[2]);
#if (LONG_MAX > INT_MAX || LONG_MIN < INT_MIN)
    if (source < INT_MIN || source > INT_MAX)
        return 1; /* exceeded domain boundaries if sizeof(int) < sizeof(long) */
#endif
    power = (int)source;
    source = significand;
    if (power < 0) {
        do
            source /= 2;
        while (++power < 0);
    } else {
        while (power-- > 0)
            source *= 2;
    }
    i_result = source;
    return 0;
}
int ibradix(int argc, char* argv[])
{
    real source;
    int power;

    if (argc < 2)
        return -1;
    source = strtor(argv[1]);
    frexp(source, &power);
    i_result = power;
    return 0;
}
int icomp(int argc, char* argv[])
{
    integer n0, n1;

    if (argc < 3)
        return -1;
    n0 = strtoi(argv[1]);
    n1 = strtoi(argv[2]);
    if (n0 < n1)
        i_result = -1;
    else if (n0 == n1)
        i_result =  0;
    else
        i_result = +1;
    return 0;
}
int imean(int argc, char* argv[])
{
    integer answer;
    register int i;

    answer = 0;
    if (argc < 2)
        return -1;
    for (i = 1; i < argc; i++)
        answer += strtoi(argv[i]);
    i_result = answer / (argc - 1);
    return 0;
}
int imedian(int argc, char* argv[])
{
    integer* constants;
    register size_t i, limit;

    if (argc < 2)
        return -1;
    limit = (unsigned int)(argc) - 1; /* argc isn't negative, so this fits. */
    constants = (integer*)malloc(sizeof(integer) * limit);
    if (constants == NULL)
        return 1;

    for (i = 0; i < limit; i++)
        constants[i] = strtoi(argv[i + 1]);
    qsort(constants, limit, sizeof(integer), (qsort_cmp_func)iqsort_cmp);

    if (limit % 2) { /* An odd number of constants has only one middle const. */
        i = (limit - 1) >> 1;
        i_result = constants[i];
    } else { /* An even-numbered list of constants has two middles. */
        i = (limit >> 1) - 1;
        i_result = (constants[i + 0] + constants[i + 1]) / 2;
    }
    free(constants);
    return 0;
}
int imode(int argc, char* argv[])
{
    integer_list* unique_numbers;
    integer* constants;
    whole_number most_repeats;
    register size_t i, j, limit, number_of_unique_entries;

    if (argc < 2)
        return -1;
    limit = (unsigned int)(argc) - 1;
    constants = (integer*)malloc(sizeof(integer) * limit);
    if (constants == NULL)
        return 1;

    for (i = 0; i < limit; i++)
        constants[i] = strtoi(argv[i + 1]);
    qsort(constants, limit, sizeof(integer), (qsort_cmp_func)iqsort_cmp);

    number_of_unique_entries = 1;
    for (i = 1; i < limit; i++)
        if (constants[i] != constants[i - 1])
            ++(number_of_unique_entries);
    unique_numbers = (integer_list*)
        calloc(number_of_unique_entries, sizeof(integer_list));
    if (unique_numbers == NULL) {
        free(constants);
        return 1;
    }

    unique_numbers[0].constant = constants[0];
    for (j = 0, i = 1; i < limit; i++)
        if (constants[i] != constants[i - 1])
            unique_numbers[++j].constant = constants[i];
        else
            ++(unique_numbers[j].repeats);
    free(constants);

    i_result     = unique_numbers[0].constant;
    most_repeats = unique_numbers[0].repeats;
    for (j = 1; j < number_of_unique_entries; j++)
        if (most_repeats < unique_numbers[j].repeats) {
            most_repeats = unique_numbers[j].repeats;
            i_result = unique_numbers[j].constant;
        }
    free(unique_numbers);
    if (most_repeats == 0)
        return 1; /* This data set has no mode.  All constants appear once. */
    return 0;
}
int irange(int argc, char* argv[])
{
    integer greatest, least, source;
    register int i;

    if (argc < 2)
        return -1;
    greatest = least = 0;
    for (i = 1; i < argc; i++) {
        source = strtoi(argv[i]);
        if (least > source || i == 1)
            least = source;
        if (greatest < source || i == 1)
            greatest = source;
    }
    i_result = greatest - least;
    return 0;
}
int iasin(int argc, char* argv[])
{
    real answer, old_result;
    int error_status;

    old_result = r_result;
    error_status = rasin(argc, argv);
    answer = r_result * 180 / pi();
    r_result = old_result;

    i_result = ftoi_round(answer);
    return (error_status);
}
int iacos(int argc, char* argv[])
{
    real answer, old_result;
    int error_status;

    old_result = r_result;
    error_status = racos(argc, argv);
    answer = r_result * 180 / pi();
    r_result = old_result;

    i_result = ftoi_round(answer);
    return (error_status);
}
int iatan(int argc, char* argv[])
{
    real answer, old_result;
    int error_status;

    old_result = r_result;
    error_status = ratan(argc, argv);
    answer = r_result * 180 / pi();
    r_result = old_result;

    i_result = ftoi_round(answer);
    return (error_status);
}

int iexit(int argc, char* argv[])
{
    int status_code;

    i_result = EXIT_SUCCESS;
    if (argc >= 2)
        i_result = strtoi(argv[1]);
#if (LONG_MIN < INT_MIN || LONG_MAX > INT_MAX)
    if (i_result < INT_MIN || i_result > INT_MAX)
        exit(EXIT_FAILURE);
#endif
    status_code = (int)(i_result);
    if (status_code)
        exit(status_code);
    return 0x7FFF;
}
