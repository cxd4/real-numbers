#include <float.h>
#include <signal.h>

#include "calc.h"
#include <math.h>

#include "strtor.h"
whole_number w_result;

int wadd(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtow(argv[i]);
        answer += source;
    }
    w_result = answer;
    return 0;
}
int wsubtract(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
        answer -= source;
    }
    w_result = answer;
    return 0;
}
int wmultiply(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtow(argv[i]);
        answer *= source;
    }
    w_result = answer;
    return 0;
}
int wdivide(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
#ifndef SIGFPE
        if (source == 0) {
            answer = ULONG_MAX;
            continue;
        }
#endif
        answer /= source;
    }
    w_result = answer;
    return 0;
}
int wmodulo(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
#ifndef SIGFPE
        if (source == 0)
            return 1;
#endif
        answer %= source;
    }
    w_result = answer;
    return 0;
}
int wpower(int argc, char* argv[])
{
    whole_number answer;
    whole_number base, power;
    register int i;

    if (argc < 2)
        return -1;
    base = strtow(argv[1]);
    power = 1;
    for (i = 2; i < argc; i++)
        power *= strtow(argv[i]);

    answer = +1;
    while (power-- != 0)
        answer *= base;
    w_result = answer;
    return 0;
}
int wbexp(int argc, char* argv[])
{
    whole_number significand;
    integer source;
    unsigned int shift_amount;

    if (argc < 3)
        return -1;
    significand = strtow(argv[1]);
    source      = strtoi(argv[2]);
#if (LONG_MAX > INT_MAX || LONG_MIN < INT_MIN)
    if (source < INT_MIN || source > INT_MAX)
        return 1; /* exceeded domain boundaries if sizeof(int) < sizeof(long) */
#endif
    shift_amount = (unsigned int)(source < 0 ? -source : +source);
    w_result = significand;
    if (source < 0)
        w_result >>= shift_amount;
    else
        w_result <<= shift_amount;
    return 0;
}
int wbradix(int argc, char* argv[])
{
    real significand, source;
    int power;

    if (argc < 1)
        return -1;
    w_result = FLT_RADIX;
    if (argc < 3)
        return 0; /* Specify an optional argv[1] to test frexp() reciprocity. */
    source = strtor(argv[1]);
    significand = frexp(source, &power);
    if (pow(FLT_RADIX, power) * significand != source)
        return 1; /* Test failed...but why? */
    return 0;
}
int wfact(int argc, char* argv[])
{
    whole_number answer, old_answer;
    register whole_number i, j, n;

    if (argc < 2)
        return -1;
    n = strtow(argv[1]);

    answer = 1;
    for (i = 1; i <= n; i++) {
        old_answer = answer;
        for (j = 1; j < i; j++) {
            answer += old_answer;
            if (answer < old_answer)
                return 1; /* unsigned overflow */
        }
    }
    w_result = answer;
    return 0;
}
int wcomp(int argc, char* argv[])
{
    whole_number n0, n1;

    if (argc < 3)
        return -1;
    n0 = strtow(argv[1]);
    n1 = strtow(argv[2]);
    w_result = (n0 < n1) ? 1 : 0;
    return 0;
}
