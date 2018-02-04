#include <float.h>
#include <signal.h>

#include "strtor.h"
#include <stdio.h>

#include "calc.h"
#include <math.h>

int add(int argc, char* argv[])
{
    double answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtor(argv[i]);
        answer += source;
    }
    printf("%g\n", answer);
    return 0;
}
int iadd(int argc, char* argv[])
{
    long answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtoi(argv[i]);
        answer += source;
    }
    printf("%li\n", answer);
    return 0;
}
int wadd(int argc, char* argv[])
{
    unsigned long answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtow(argv[i]);
        answer += source;
    }
    printf("%lu\n", answer);
    return 0;
}

int subtract(int argc, char* argv[])
{
    double answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtor(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtor(argv[i]);
        answer -= source;
    }
    printf("%g\n", answer);
    return 0;
}
int isubtract(int argc, char* argv[])
{
    long answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtoi(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtoi(argv[i]);
        answer -= source;
    }
    printf("%li\n", answer);
    return 0;
}
int wsubtract(int argc, char* argv[])
{
    unsigned long answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
        answer -= source;
    }
    printf("%lu\n", answer);
    return 0;
}

int multiply(int argc, char* argv[])
{
    double answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtor(argv[i]);
        answer *= source;
    }
    printf("%g\n", answer);
    return 0;
}
int imultiply(int argc, char* argv[])
{
    long answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtoi(argv[i]);
        answer *= source;
    }
    printf("%li\n", answer);
    return 0;
}
int wmultiply(int argc, char* argv[])
{
    unsigned long answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtow(argv[i]);
        answer *= source;
    }
    printf("%lu\n", answer);
    return 0;
}

int divide(int argc, char* argv[])
{
    double answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtor(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtor(argv[i]);
#ifndef SIGFPE
        if (source == 0)
            source = +(DBL_MIN);
#endif
        answer /= source;
    }
    printf("%g\n", answer);
    return 0;
}
int idivide(int argc, char* argv[])
{
    long answer, source;
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
        answer /= source;
    }
    printf("%li\n", answer);
    return 0;
}
int wdivide(int argc, char* argv[])
{
    unsigned long answer, source;
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
    printf("%lu\n", answer);
    return 0;
}

const math_operation op_functions[] = {
     add     , iadd     , wadd     ,
     subtract, isubtract, wsubtract,
     multiply, imultiply, wmultiply,
     divide  , idivide  , wdivide  ,
};
